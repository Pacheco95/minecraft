#version 330 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec4 in_color;
layout(location = 3) in vec2 in_tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_normal;
out vec4 v_color;
out vec2 v_tex_coord;

void main() {
  gl_Position = u_projection * u_view * u_model * vec4(in_pos, 1.0);
  v_normal = mat3(transpose(inverse(u_model))) * in_normal;
  v_color = in_color;
  v_tex_coord = in_tex_coord;
}
