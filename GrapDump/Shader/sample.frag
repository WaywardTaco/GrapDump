
#version 330 core

// Texture Stuff
uniform sampler2D tex0; 
in vec2 texCoord; 

in vec3 normCoord;
in vec3 fragPos;

uniform vec3 lightDir;
uniform vec3 lightColor;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

uniform float brightness;

out vec4 FragColor; 

void main() {

	vec3 lightColorMod = lightColor * brightness;

	vec3 pointNorm = normalize(normCoord);
	vec3 lightDirNorm = normalize(lightDir);

	float diff = max(dot(pointNorm, lightDirNorm), 0.0);
	vec3 diffuse = diff * lightColorMod;

	// Basic ambient calculations
	vec3 ambientCol = ambientColor * ambientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDirNorm, pointNorm);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColorMod;

	// Adjusts the color of the fragment depending on coordinate
	FragColor = vec4(diffuse + ambientCol + specColor, 1.0) * texture(tex0, texCoord);

}
