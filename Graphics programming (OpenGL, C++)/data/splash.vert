#version 140

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 position;           // vertex position in world space
in vec2 texCoord;           // incoming texture coordinates

smooth out vec2 fragTexCoord; // outgoing vertex texture coordinates

void main() {

  // vertex position after the projection (gl_Position is predefined output variable)
  gl_Position = projection * view * model * vec4(position, 1);   // outgoing vertex in clip coordinates

  // outputs entering the fragment shader
  fragTexCoord = texCoord;
}
