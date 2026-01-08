#version 330 core

out vec4 FragColor;

// Light types index (matches LightType enum class)
const uint LIGHT_DIRECTIONAL = 0u;
const uint LIGHT_POINT = 1u;
const uint LIGHT_SPOT = 2u;

in VsOut {
  vec3 fragPos;
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
  Light light;
};

uniform Material uMaterial;
uniform World uWorld; // Scene/Global Uniforms

void main() {
  FragColor = uWorld.light.color;
}
