
#version 330 core

in mat3 TBN;
in vec2 texCoord; 
in vec3 normCoord;
in vec3 fragPos;

// Structs for lights
struct DirLight {
	vec3 direction;

	vec3 color;
	float brightness;

	vec3 ambientCol;
	float ambientStr;

	float specPhong;
	float specStr;
};

struct PointLight {
	vec3 position;

	vec3 color;
	float brightness;

	vec3 ambientCol;
	float ambientStr;

	float specPhong;
	float specStr;
};

// Function Predefs
vec3 calcDirLighting(DirLight light, vec3 pointNorm, vec3 viewDir);
vec3 calcPointLighting(PointLight light, vec3 pointNorm, vec3 fragPos, vec3 viewDir);

// Uniforms
uniform vec3 cameraPos;

uniform sampler2D tex0; 
uniform vec3 modelBaseColor;
uniform bool modelHasTexture;

uniform sampler2D norm_tex;
uniform bool modelHasNormMap;

uniform DirLight dirLight;
uniform PointLight pointLight;

out vec4 FragColor; 

void main() {
	// Skip Transparent Pixels
	vec4 pixelColor = texture(tex0, texCoord);
	if(pixelColor.a < 0.1){
		discard;
	}
	if(!modelHasTexture){
		FragColor = vec4(modelBaseColor, 1.0);
		if(FragColor.r > 0.1)
			FragColor.r = 0.1;
		if(FragColor.b > 0.1)
			FragColor.b = 0.1;

		return;
	}

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 pointNorm = normalize(normCoord);

	if(modelHasNormMap){
		pointNorm = texture(norm_tex, texCoord).rgb;
		pointNorm = normalize(pointNorm * 2.0 - 1.0); 
		pointNorm = normalize(TBN * pointNorm);
	} 

	vec3 lighting = vec3(0.0);
	lighting = lighting + calcDirLighting(dirLight, pointNorm, viewDir);
	lighting = lighting + calcPointLighting(pointLight, pointNorm, fragPos, viewDir);

	FragColor = vec4(lighting, 1.0) * texture(tex0, texCoord);
	if(FragColor.r > 0.1)
		FragColor.r = 0.1;
	if(FragColor.b > 0.1)
		FragColor.b = 0.1;
}

vec3 calcDirLighting(DirLight light, vec3 pointNorm, vec3 viewDir){
	// Light Dir based on light direcection only
	vec3 lightDir = normalize(-light.direction);
	
	float diff = max(dot(pointNorm, lightDir), 0.0);
	vec3 diffuse = light.color * diff * light.brightness ;

	vec3 ambient = light.ambientCol * light.ambientStr ;
	
	vec3 reflectDir = reflect(-lightDir, pointNorm) ;
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), light.specPhong) ;
	vec3 specular = spec * light.specStr * light.color ;

	vec3 lighting = vec3(0.0);
	lighting += (diffuse + ambient + specular);

	return lighting;
}

vec3 calcPointLighting(PointLight light, vec3 pointNorm, vec3 fragPos, vec3 viewDir){
	// Distance calculated for lighting adjustments
	float distance = length(light.position - fragPos);
	float distMult = 1.0 / (distance * distance);
	
	// Light Dir based on fragment & light positions
	vec3 lightDir = normalize(light.position - fragPos);
	
	float diff = max(dot(pointNorm, lightDir), 0.0);
	vec3 diffuse = light.color * diff * light.brightness ;

	vec3 ambient = light.ambientCol * light.ambientStr ;
	
	vec3 reflectDir = reflect(-lightDir, pointNorm) ;
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), light.specPhong) ;
	vec3 specular = spec * light.specStr * light.color ;

	// Adjustment of light based on distance
	diffuse *= distMult;
	ambient *= distMult;
	specular *= distMult;

	vec3 lighting = vec3(0.0);
	lighting += (diffuse + ambient + specular);

	return lighting;
}