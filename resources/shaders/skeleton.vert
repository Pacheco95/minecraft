
#version 330 core

// Matches VertexAttributeIndex enum in Mesh.h
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoords;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBitangent;

out VsOut {
  vec3 fragPos;
  vec4 color;
  vec2 texCoords;
  vec3 normal;
  mat3 TBN; // For Normal Mapping
}
vsOut;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
  vsOut.fragPos = vec3(uModel * vec4(aPosition, 1.0));
  vsOut.color = aColor;
  vsOut.texCoords = aTexCoords;

  // Normal matrix to handle non-uniform scaling
  mat3 normalMatrix = transpose(inverse(mat3(uModel)));
  vsOut.normal = normalize(normalMatrix * aNormal);

  // Construct TBN matrix for Normal Mapping
  vec3 T = normalize(normalMatrix * aTangent);
  vec3 B = normalize(normalMatrix * aBitangent);
  vec3 N = normalize(normalMatrix * aNormal);
  vsOut.TBN = mat3(T, B, N);

  gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
