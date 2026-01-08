#version 330 core

out vec4 FragColor;

in VS_OUT {
  vec3 FragPos;
  vec4 Color;
  vec2 TexCoords;
  vec3 Normal;
  mat3 TBN;
}
fs_in;

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
  vec3 position;
  vec3 direction;

  vec4 ambientColor;
  vec4 diffuseColor;
  vec4 specularColor;
};

uniform Material uMaterial;

// Scene/Global Uniforms
uniform vec3 uViewPosition;
uniform Light uLight;

void main() {
  vec3 lightDir = normalize(uLight.direction);
  //  vec3 lightDir = normalize(vec3(0.0, 1.0, 0.0));

  // 1. Sample Normal Map or use Vertex Normal
  vec3 normal = fs_in.Normal;
  // If a normal map is bound, use the TBN matrix to transform sampled normal
  vec3 sampledNormal = texture(uMaterial.normalTexture, fs_in.TexCoords).rgb;
  sampledNormal = normalize(sampledNormal * 2.0 - 1.0); // Map [0,1] to [-1,1]
  normal = normalize(fs_in.TBN * sampledNormal);

  // 2. Diffuse Lighting
  float diff = max(dot(normal, lightDir), 0.0);
  //  vec4 diffuse = diff * texture(uMaterial.diffuseTexture, fs_in.TexCoords) * uMaterial.diffuseColor;
  vec4 diffuse = texture(uMaterial.diffuseTexture, fs_in.TexCoords);

  // 3. Specular Lighting (Blinn-Phong)
  vec3 viewDir = normalize(uViewPosition - fs_in.FragPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), uMaterial.shininess);
  vec4 specular = spec * texture(uMaterial.specularTexture, fs_in.TexCoords) * uMaterial.specularColor;

  // 4. Final Color
  //  FragColor = (diffuse + specular) * fs_in.Color;
  FragColor = vec4(diffuse);
}
