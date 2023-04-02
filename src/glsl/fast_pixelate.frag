#version 330

uniform sampler2D screen;
uniform float scale;

in vec2 fragTexCoord;

layout (location = 0) out vec4 result;

void main() {
  vec2 pixelCoord = floor(textureSize(screen, 0) * scale * fragTexCoord);
  vec2 sampleLocation = pixelCoord / (textureSize(screen, 0) * scale);
  result = texture(screen, sampleLocation);
}
