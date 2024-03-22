
#version 330 core

// Normal Maps Stuff
uniform sampler2D norm_tex;
uniform bool modelHasNormMap;
in mat3 TBN;

// Texture Stuff
uniform sampler2D tex0; 
in vec2 texCoord; 

in vec3 normCoord;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

uniform float brightness;

uniform bool modelHasTexture;
uniform vec3 modelBaseColor;

out vec4 FragColor; 

void main() {
	// Skips over transparent pixels
	vec4 pixelColor = texture(tex0, texCoord);
	if(pixelColor.a < 0.1){ // 0.1 is the alpha cutoff value
		discard;
	}

	float distance = max(length(fragPos - lightPos), 0.0);

	float intensity = brightness / (distance * distance);

	vec3 lightColorMod = lightColor * intensity;

	vec3 pointNorm;

	if(modelHasNormMap){
		// RGB converts to XYZ normal data
		pointNorm = texture(norm_tex, texCoord).rgb;
		// Adjusts 0 to 1.0 range to a range of -1.0 to 1.0 normals
		pointNorm = normalize(pointNorm * 2.0 - 1.0); 
		pointNorm = normalize(TBN * pointNorm);
	} else {
		pointNorm = normalize(normCoord);
	}

	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(pointNorm, lightDir), 0.0);

	vec3 diffuse = diff * lightColorMod;

	// Basic ambient calculations
	vec3 ambientCol = ambientColor * ambientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, pointNorm);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColorMod;

	// Adjusts the color of the fragment depending on coordinate
	if(modelHasTexture){
		FragColor = vec4(diffuse + ambientCol + specColor, 1.0) * texture(tex0, texCoord);
	} else {
		FragColor = vec4(modelBaseColor, 1.0);
	}

}
