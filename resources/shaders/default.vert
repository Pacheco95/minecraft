#version 330 core

// Matches VertexAttributeIndex enum in Mesh.h
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoords;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBitangent;

out VS_OUT {
  vec3 FragPos;
  vec4 Color;
  vec2 TexCoords;
  vec3 Normal;
  mat3 TBN; // For Normal Mapping
}
vs_out;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
  vs_out.FragPos = vec3(uModel * vec4(aPos, 1.0));
  vs_out.Color = aColor;
  vs_out.TexCoords = aTexCoords;

  // Normal matrix to handle non-uniform scaling
  mat3 normalMatrix = transpose(inverse(mat3(uModel)));
  vs_out.Normal = normalize(normalMatrix * aNormal);

  // Construct TBN matrix for Normal Mapping
  vec3 T = normalize(normalMatrix * aTangent);
  vec3 B = normalize(normalMatrix * aBitangent);
  vec3 N = normalize(normalMatrix * aNormal);
  vs_out.TBN = mat3(T, B, N);

  gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
