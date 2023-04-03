#version 330

uniform sampler2D screen;
uniform int radius;

in vec2 fragTexCoord;

layout (location = 0) out vec4 result;

void main() {
  vec3 sqrColorSum = vec3(0);

  vec2 stepSize = vec2(1.0) / textureSize(screen, 0);
  vec2 sampleLocation = fragTexCoord + stepSize / 2.0;

  for (int y = -radius + 1; y < radius; ++y) {
    for (int x = -radius + 1; x < radius; ++x) {
      vec2 off = stepSize * vec2(x, y);
      vec3 color = texture(screen, sampleLocation + off).rgb;
      sqrColorSum += color * color;
    }
  }

  float area = (radius * 2 + 1);
  area *= area;

  vec4 averageColor = vec4(sqrt(sqrColorSum / area), 1.0);
  result = averageColor;
}
