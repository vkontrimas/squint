#version 330

uniform sampler2D screen;

in vec2 fragTexCoord;

layout (location = 0) out vec4 result;

void main() {
  result = texture(screen, fragTexCoord);
}
