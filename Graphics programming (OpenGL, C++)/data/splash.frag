#version 140

uniform float time;           // used for simulation of moving lights (such as sun) and to select proper animation frame
uniform mat4 view;         // view (camera) transform --> world to eye coordinates
uniform sampler2D texSampler; // sampler for texture access

smooth in vec2 fragTexCoord;    // fragment texture coordinates

out vec4 fragColor;             // outgoing fragment color

uniform ivec2 pattern = ivec2(5, 10);
uniform float frameDuration = 0.1f;


vec4 sampleTexture(int frame) {

  vec2 offset = vec2(1.0) / vec2(pattern);

  vec2 texCoordBase = fragTexCoord / vec2(pattern);
  vec2 texOffset = vec2(frame % pattern.x, pattern.y - 1 - (frame / pattern.x)) * offset;
  texOffset.y -= 0.002;
  vec2 texCoord = texCoordBase + texOffset;

  return texture(texSampler, texCoord);
}

void main() {
  // frame of the texture to be used for explosion drawing 
  int frame = int(time / frameDuration);

  // sample proper frame of the texture to get a fragment color  
  fragColor = sampleTexture(frame);
}
