#version 330

uniform sampler2D downscaled;

in vec2 fragTexCoord;

layout (location = 0) out vec4 result;

void main() {
  result = texture(downscaled, fragTexCoord);
}
