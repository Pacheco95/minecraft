#version 330 core

#define DEBUG

#ifdef DEBUG
vec4 useUniforms();
#endif

// Light types index (matches LightType enum class)
#define LIGHT_DIRECTIONAL 0u
#define LIGHT_POINT 1u
#define LIGHT_SPOT 2u

#define MAX_LIGHT_SOURCES 5

out vec4 FragColor;

in VsOut {
  vec3 fragWorldPos;
  vec4 color;
  vec2 texCoords;
  vec3 normal;
  mat3 TBN; // For Normal Mapping
}
fsIn;

struct Material {
  vec4 diffuseColor;
  vec4 ambientColor;
  vec4 specularColor;
  vec4 emissiveColor;
  vec4 transparentColor;
  vec4 reflectiveColor;

  sampler2D diffuseTexture;
  sampler2D specularTexture;
  sampler2D normalTexture;

  float opacity;
  float shininess;
  float shininessStrength;
  float reflectivity;
  float refractionIndex;
  float bumpScaling;
  float transparencyFactor;
};

struct Light {
  uint type;
  vec4 color;
  float intensity;
  vec3 position;
  vec3 direction;
  float constant;
  float linear;
  float quadratic;
  float innerCutoff;
  float outerCutoff;
};

struct World {
  vec3 viewPosition;
  Light lights[MAX_LIGHT_SOURCES];
  uint lightSourceCount;
};

uniform Material uMaterial;
uniform World uWorld; // Scene/Global Uniforms

const float PI = 3.14159265359;

// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal
// mapping the usual way for performance anyways; I do plan make a note of this
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap() {
  vec3 tangentNormal = texture(uMaterial.normalTexture, fsIn.texCoords).xyz * 2.0 - 1.0;

  vec3 Q1 = dFdx(fsIn.fragWorldPos);
  vec3 Q2 = dFdy(fsIn.fragWorldPos);
  vec2 st1 = dFdx(fsIn.texCoords);
  vec2 st2 = dFdy(fsIn.texCoords);

  vec3 N = normalize(fsIn.normal);
  vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
  vec3 B = -normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);

  return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float nom = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;

  float nom = NdotV;
  float denom = NdotV * (1.0 - k) + k;

  return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2 = GeometrySchlickGGX(NdotV, roughness);
  float ggx1 = GeometrySchlickGGX(NdotL, roughness);

  return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {
#ifdef DEBUG
  vec4 x = useUniforms();
#else
  vec4 x = vec4(1.0);
#endif

  vec3 albedo = pow(texture(uMaterial.diffuseTexture, fsIn.texCoords).rgb, vec3(2.2));
  float metallic = texture(uMaterial.specularTexture, fsIn.texCoords).r;
  //  float roughness = texture(roughnessMap, fsIn.texCoords).r;
  //  float ao = texture(aoMap, fsIn.texCoords).r;
  float roughness = 0.5;
  float ao = 0.2;

  vec3 N = getNormalFromMap();
  vec3 V = normalize(uWorld.viewPosition - fsIn.fragWorldPos);

  // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
  // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
  vec3 F0 = vec3(0.04);
  F0 = mix(F0, albedo, metallic);

  // reflectance equation
  vec3 Lo = vec3(0.0);
  for (int i = 0; i < 4; ++i) {
    // calculate per-light radiance
    vec3 L = normalize(uWorld.lights[i].position - fsIn.fragWorldPos);
    vec3 H = normalize(V + L);
    float distance = length(uWorld.lights[i].position - fsIn.fragWorldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = uWorld.lights[i].color.rgb * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    Lo += (kD * albedo / PI + specular) * radiance *
          NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
  }

  // ambient lighting (note that the next IBL tutorial will replace
  // this ambient lighting with environment lighting).
  vec3 ambient = vec3(0.03) * albedo * ao;

  vec3 color = ambient + Lo;

  // HDR tonemapping
  color = color / (color + vec3(1.0));
  // gamma correct
  color = pow(color, vec3(1.0 / 2.2));

  FragColor = vec4(color, 1.0);
}

#ifdef DEBUG
vec4 useUniforms() {
  return vec4(uWorld.viewPosition, 1.0f) + vec4(uWorld.lightSourceCount) + vec4(uWorld.lights[0].intensity) +
         vec4(uWorld.lights[0].position, 1.0f) + vec4(uWorld.lights[0].direction, 1.0f) +
         vec4(uWorld.lights[0].constant) + vec4(uWorld.lights[0].linear) + vec4(uWorld.lights[0].quadratic) +
         vec4(uWorld.lights[0].innerCutoff) + vec4(uWorld.lights[0].outerCutoff) + +vec4(uMaterial.refractionIndex) +
         vec4(uMaterial.opacity) + vec4(uMaterial.shininess) + vec4(uWorld.viewPosition, 1.0f) +
         vec4(uWorld.lightSourceCount) + vec4(uWorld.lights[0].color) + vec4(uWorld.lights[0].intensity) +
         vec4(uWorld.lights[0].position, 1.0f) + vec4(uWorld.lights[0].direction, 1.0f) +
         vec4(uWorld.lights[0].constant) + vec4(uWorld.lights[0].linear) + vec4(uWorld.lights[0].quadratic) +
         vec4(uWorld.lights[0].innerCutoff) + vec4(uWorld.lights[0].outerCutoff) +
         texture(uMaterial.normalTexture, vec2(0.0, 0.02)) + texture(uMaterial.specularTexture, vec2(0.0, 0.02)) +
         texture(uMaterial.diffuseTexture, vec2(0.0, 0.02));
}
#endif
