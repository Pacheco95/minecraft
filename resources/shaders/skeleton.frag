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

void main() {
#ifdef DEBUG
  vec4 x = useUniforms();
#else
  vec4 x = vec4(1.0);
#endif

  FragColor = texture(uMaterial.diffuseTexture, fsIn.texCoords);
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
