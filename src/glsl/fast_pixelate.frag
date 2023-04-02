#version 330

uniform sampler2D screen;
uniform float pixelSize;

in vec2 fragTexCoord;

layout (location = 0) out vec4 result;

void main() {
  vec2 screenSize = textureSize(screen, 0);
  vec2 reducedSize = screenSize / pixelSize;
  vec2 sampleCoord = floor(reducedSize * fragTexCoord) / reducedSize;
  result = texture(screen, sampleCoord);
}
