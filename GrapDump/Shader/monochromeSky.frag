
#version 330 core

out vec4 FragColor;

in vec3 texCoord;

uniform samplerCube skybox;

void main(){
	FragColor = texture(skybox, texCoord);
	if(FragColor.r > 0.1)
		FragColor.r = 0.1;
	if(FragColor.b > 0.1)
		FragColor.b = 0.1;
}