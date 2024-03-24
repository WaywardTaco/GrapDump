
#version 330 core

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

vec3 calcDirLighting(DirLight light, vec3 pointNorm, vec3 viewDir);
vec3 calcPointLighting(PointLight light, vec3 pointNorm, vec3 fragPos, vec3 viewDir);

in vec3 fragPos;
in vec3 normCoord;
in vec2 texCoord; 

uniform bool modelHasTexture;
uniform vec3 modelBaseColor;
uniform sampler2D tex0; 

uniform vec3 cameraPos;

uniform DirLight dirLight;
uniform PointLight pointLight;

out vec4 FragColor; 

void main() {
	if(!modelHasTexture){
		FragColor = vec4(modelBaseColor, 1.0);
		return;
	}

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 pointNorm = normalize(normCoord);

	vec3 lighting = vec3(0.0);
	lighting += calcDirLighting(dirLight, pointNorm, viewDir);
	lighting += calcPointLighting(pointLight, pointNorm, fragPos, viewDir);

	FragColor = vec4(lighting, 1.0) * texture(tex0, texCoord);
}

vec3 calcDirLighting(DirLight light, vec3 pointNorm, vec3 viewDir){
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
	float distance = length(light.position - fragPos);
	float distMult = 1.0 / (distance * distance);
	
	vec3 lightDir = normalize(light.position - fragPos);
	
	float diff = max(dot(pointNorm, lightDir), 0.0);
	vec3 diffuse = light.color * diff * light.brightness ;

	vec3 ambient = light.ambientCol * light.ambientStr ;
	
	vec3 reflectDir = reflect(-lightDir, pointNorm) ;
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), light.specPhong) ;
	vec3 specular = spec * light.specStr * light.color ;

	diffuse *= distMult;
	ambient *= distMult;
	specular *= distMult;

	vec3 lighting = vec3(0.0);
	lighting += (diffuse + ambient + specular);

	return lighting;
}