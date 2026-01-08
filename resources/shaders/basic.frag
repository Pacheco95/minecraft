#version 330 core

out vec4 FragColor;

struct Material {
  sampler2D diffuseTexture;
  sampler2D specularTexture;
  float shininess;
};

struct Light {
  vec3 position;

  vec4 ambientColor;
  vec4 diffuseColor;
  vec4 specularColor;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 uViewPosition;
uniform Material uMaterial;
uniform Light uLight;

void main() {
  // ambient
  vec4 ambient = uLight.ambientColor * texture(uMaterial.diffuseTexture, TexCoords);

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(uLight.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec4 diffuse = uLight.diffuseColor * diff * texture(uMaterial.diffuseTexture, TexCoords);

  // specular
  vec3 viewDir = normalize(uViewPosition - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
  vec4 specular = uLight.specularColor * spec * texture(uMaterial.specularTexture, TexCoords);

  FragColor = ambient + diffuse + specular;
}
