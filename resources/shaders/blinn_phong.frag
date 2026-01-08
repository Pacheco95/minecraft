#version 330 core

out vec4 FragColor;

in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
}
fs_in;

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

uniform Material uMaterial;
uniform Light uLight;
uniform vec3 uViewPosition;

const bool blinn = true;

void main() {
  vec3 color = texture(uMaterial.diffuseTexture, fs_in.TexCoords).rgb;
  // ambient
  vec3 ambient = 0.05 * color;
  // diffuse
  vec3 lightDir = normalize(uLight.position - fs_in.FragPos);
  vec3 normal = normalize(fs_in.Normal);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * color;
  // specular
  vec3 viewDir = normalize(uViewPosition - fs_in.FragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = 0.0;
  if (blinn) {
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
  } else {
    vec3 reflectDir = reflect(-lightDir, normal);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
  }
  vec3 specular = vec3(0.3) * spec; // assuming bright white light color
  FragColor = vec4(ambient + diffuse + specular, 1.0);
}
