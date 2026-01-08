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

// Material Textures (Mapped in Material.cpp)
uniform sampler2D texture_diffuse1;  // Unit 0
uniform sampler2D texture_specular1; // Unit 1
uniform sampler2D texture_normal1;   // Unit 2

// Material Uniforms (Mapped in Material.h)
uniform vec4 uDiffuseColor = vec4(1.0, 1.0, 1.0, 1.0); // Default to White
uniform vec4 uSpecularColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform float uShininess = 32.0; // Standard plastic-like shine
uniform float uOpacity = 1.0;    // Default Opaque
uniform float uBumpScaling = 1.0;

// Scene/Global Uniforms
uniform vec3 uViewPosition;
uniform vec3 uLightPosition;
uniform vec3 uLightDirection;

void main() {
  vec3 lightDir = normalize(uLightDirection);
  //  vec3 lightDir = normalize(vec3(0.0, 1.0, 0.0));

  // 1. Sample Normal Map or use Vertex Normal
  vec3 normal = fs_in.Normal;
  // If a normal map is bound, use the TBN matrix to transform sampled normal
  vec3 sampledNormal = texture(texture_normal1, fs_in.TexCoords).rgb;
  sampledNormal = normalize(sampledNormal * 2.0 - 1.0); // Map [0,1] to [-1,1]
  normal = normalize(fs_in.TBN * sampledNormal);

  // 2. Diffuse Lighting
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * texture(texture_diffuse1, fs_in.TexCoords).rgb * uDiffuseColor.rgb;

  // 3. Specular Lighting (Blinn-Phong)
  vec3 viewDir = normalize(uViewPosition - fs_in.FragPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), uShininess);
  vec3 specular = spec * texture(texture_specular1, fs_in.TexCoords).rgb * uSpecularColor.rgb;

  // 4. Final Color
  vec3 result = (diffuse + specular) * fs_in.Color.rgb;
  FragColor = vec4(result, uOpacity);
}
