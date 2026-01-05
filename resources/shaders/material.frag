#version 330 core

in vec3 oNormal;
in vec4 oColor;
in vec2 oTexCoord;

uniform sampler2D u_texture;

out vec4 FragColor;

void main() {
  vec4 texColor = texture(u_texture, oTexCoord);
  FragColor = texColor * oColor;
}
