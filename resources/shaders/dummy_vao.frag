#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;

void main() {
  float dist = length(vUV - 0.5);
  float pulse = sin(uTime * 2.0) * 0.5 + 0.5;

  vec3 color = mix(vec3(0.05, 0.1, 0.2), vec3(0.2, 0.8, 1.0), smoothstep(0.3 + pulse * 0.1, 0.0, dist));

  FragColor = vec4(color, 1.0);
}
