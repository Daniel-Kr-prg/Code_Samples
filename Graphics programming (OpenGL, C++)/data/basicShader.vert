#version 140

in vec2 texCoord;
in vec3 normal;
in vec3 position;

out vec3 fragNormal;
out vec3 fragPosition;
out vec2 fragTexCoord;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

// TexCoord Transform matrix
uniform mat3 texCoordMatrix;

void main() {
  vec4 globalPosition = model * vec4(position, 1.0f);
  fragPosition = vec3(globalPosition);
  fragTexCoord = vec2(texCoordMatrix * vec3(texCoord, 1.0));
  fragNormal = mat3(normalMatrix) * normal;  
  gl_Position = projection * view * globalPosition;
}