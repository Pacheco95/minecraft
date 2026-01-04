#version 330 core

in vec3 v_normal;
in vec4 v_color;
in vec2 v_tex_coord;

uniform sampler2D u_texture;

out vec4 out_frag_color;

void main() {
  vec4 texColor = texture(u_texture, v_tex_coord);
  out_frag_color = texColor * v_color;
}
