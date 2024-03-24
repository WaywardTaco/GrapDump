
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 aTex;

uniform mat4 projection ;
uniform mat4 view ;
uniform mat4 transform ;

out vec3 fragPos;
out vec3 normCoord;
out vec2 texCoord; 

void main () {
	// gl_Position is predefined variable that stores the final positional data of the the point
	gl_Position = projection * view * transform * vec4 (aPos, 1.0); 
	fragPos = vec3(transform * vec4(aPos, 1.0));

	normCoord = mat3(transpose(inverse(transform))) * vertexNormal ;
	texCoord = aTex;
	
}
