#version 140

out vec4 fragmentColor;

in vec3 texCoord;

uniform samplerCube skySampler;

// Fog
uniform float fogHeightStart;
uniform float fogHeightEnd;
uniform vec3 fogColor;

float getFog(float height)
{
    if (height >= fogHeightStart) return 0;
    if (height <= fogHeightEnd) return 1;

    return (fogHeightStart - height) / (fogHeightStart - fogHeightEnd);
}

void main()
{   
    fragmentColor = mix(texture(skySampler, texCoord), vec4(fogColor, 1.0), getFog(texCoord.y));
}