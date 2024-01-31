#version 330 core

in vec3 vFragColor;

out vec4 fFragColor;

void main() {
  float luminance = (vFragColor.r + vFragColor.g + vFragColor.b) / 3.0;
  fFragColor = vec4(luminance, luminance, luminance, 1.0);
};