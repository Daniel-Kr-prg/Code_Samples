#version 140

// Camera
uniform vec3 viewPosition;

// Material
uniform sampler2D texSampler;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
uniform bool useTexture;

// Ambient light
uniform float ambientLightIntensity;
uniform vec3 ambientLightColor;

// Directional light
uniform vec3 directionalLightDirection;
uniform float directionalLightIntensity;
uniform vec3 directionalLightColor;

// Point light
uniform float pointLightAttenuation;
uniform vec3 pointLightPosition;
uniform float pointLightIntensity;
uniform vec3 pointLightColor;

// Spot light
uniform float spotLightAttenuation;
uniform vec3 spotLightPosition;
uniform vec3 spotLightDirection;
uniform float spotLightIntensity;
uniform vec3 spotLightColor;
uniform float spotLightInnerCutoff;
uniform float spotLightOuterCutoff;

// Fog
uniform float fogStart;
uniform float fogEnd;
uniform vec3 fogColor;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPosition;
out vec4 fragmentColor;

vec3 getDirectionalLight(vec3 normalizedNormal, vec3 viewDirection) {
  // Directional light
  vec3 lightDirection = normalize(-directionalLightDirection);
  
  // Diffuse
  vec3 directionalDiffuse = directionalLightIntensity * max(dot(normalizedNormal, lightDirection), 0.0) * diffuse;
  
  // Specular
  vec3 reflectDirection = reflect(-lightDirection, normalizedNormal);
  vec3 directionalSpecular = directionalLightIntensity * pow(max(dot(viewDirection, reflectDirection), 0.0), shininess) * specular;
  
  return (directionalDiffuse + directionalSpecular) * directionalLightColor;
}

vec3 getPointLight(vec3 normalizedNormal, vec3 viewDirection) {
	// Point Light
	vec3 lightVector = pointLightPosition - fragPosition;
	vec3 lightDirection = normalize(lightVector);
    float lightDistance = length(lightVector);
	
    // Light Attenuation
    float lightAttenuation = 1.0 / (lightDistance * lightDistance * pointLightAttenuation);
	
    // Diffuse
    vec3 pointDiffuse = pointLightIntensity * max(dot(normalizedNormal, lightDirection), 0.0) * diffuse * lightAttenuation;  
    
    // Specular
    vec3 reflectDirection = reflect(-lightDirection, normalizedNormal);
    vec3 pointSpecular = pointLightIntensity * pow(max(dot(viewDirection, reflectDirection), 0.0), shininess) * specular * lightAttenuation;  
        
	return (pointDiffuse + pointSpecular) * pointLightColor;
}

vec3 getSpotLight(vec3 normalizedNormal, vec3 viewDirection) {
	// Spot Light
	vec3 lightVector = spotLightPosition - fragPosition;
	vec3 lightDirection = normalize(lightVector);
    float lightDistance = length(lightVector);
	
	// Attenuation
    float lightAttenuation = 1.0 / (lightDistance * lightDistance * spotLightAttenuation);    
	
	// Cutoff
    float theta = dot(lightDirection, normalize(-spotLightDirection)); 
    float epsilon = (spotLightInnerCutoff - spotLightOuterCutoff);
    float cutoff = clamp((theta - spotLightOuterCutoff) / epsilon, 0.0, 1.0);
	
	// Diffuse
    vec3 spotDiffuse = spotLightIntensity * max(dot(normalizedNormal, lightDirection), 0.0) * diffuse * cutoff * lightAttenuation;  
    
    // Specular
    vec3 reflectDirection = reflect(-lightDirection, normalizedNormal);  
    vec3 spotSpecular = spotLightIntensity * pow(max(dot(viewDirection, reflectDirection), 0.0), shininess) * specular * cutoff * lightAttenuation;
    
	return (spotDiffuse + spotSpecular) * spotLightColor;
}

float getFog(float distance)
{
    if (distance >= fogEnd) return 1;
    if (distance <= fogStart) return 0;

    return 1 - (fogEnd - distance) / (fogEnd - fogStart);
}

void main() {
  vec4 textureColor = vec4(1.0);
  if(useTexture)
	textureColor = texture2D(texSampler, fragTexCoord);
  vec3 normalizedNormal = normalize(fragNormal);
  vec3 viewDirection = normalize(viewPosition - fragPosition);
  
  // Ambient light
  vec3 ambientLight = ambientLightColor * ambientLightIntensity * ambient;
  //fragmentColor = vec4(normalizedNormal, 1.0);
  //fragmentColor = textureColor;
  
  vec4 lightColor = vec4(getDirectionalLight(normalizedNormal, viewDirection) + getPointLight(normalizedNormal, viewDirection) + getSpotLight(normalizedNormal, viewDirection) + ambientLight, 1.0) * textureColor;  
  fragmentColor = mix(lightColor, vec4(fogColor, 1.0), getFog(length(viewPosition - fragPosition)));
 
}