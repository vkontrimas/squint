#version 330

in vec2 position;
in vec2 texCoord;

out vec2 fragTexCoord;

void main() {
  gl_Position = vec4(position.x, position.y, 0, 1);
  fragTexCoord = texCoord;
}
