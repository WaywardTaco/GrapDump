
#version 330 core

// Texture Stuff
uniform sampler2D tex0; // where the texture gets assigned to
in vec2 texCoord; // Take it in from the vertex shader, same spelling and data type as vShader

in vec3 normCoord;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

out vec4 FragColor; // Returns a Color of indv pixels, required for frag shaders

void main() {

	vec3 pointNorm = normalize(normCoord);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(pointNorm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Basic ambient calculations
	vec3 ambientCol = ambientColor * ambientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, pointNorm);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor;

	// Range 0 to 1 :  R     G     B     A
	// FragColor = vec4 (0.7f, 0.7f, 0.5f, 1.f) ; // Color of the Fragment, simple single color

	// Adjusts the color of the fragment depending on coordinate
	FragColor = vec4(diffuse + ambientCol + specColor, 1.0) * texture(tex0, texCoord);

}
