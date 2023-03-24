#version 330

uniform sampler2D screen;

in vec2 fragTexCoord;

void main() {
  gl_FragColor = texture(screen, fragTexCoord);
}
