#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoords;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBitangent;

// declare an interface block; see 'Advanced GLSL' for what these are.
out VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
}
vs_out;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
  vs_out.FragPos = aPos;
  vs_out.Normal = aNormal;
  vs_out.TexCoords = aTexCoords;
  gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
