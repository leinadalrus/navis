#version 410

uniform vec4 frag_colour;
out vec4 frag_colour;

void main() {
  frag_colour = input_colour;
}