
#version 330 core

out mat3 TBN;
out vec2 texCoord;
out vec3 normCoord;
out vec3 fragPos;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 m_tan;
layout(location = 4) in vec3 m_btan;

uniform mat4 transform ;
uniform mat4 projection ;
uniform mat4 view ;

void main () {
	// gl_Position stores final position data
	gl_Position = projection * view * transform * vec4 (aPos, 1.0); 

	texCoord = aTex;

	mat3 modelMat = mat3(transpose(inverse(transform)));

	normCoord = modelMat * vertexNormal ;

	vec3 Tangent = normalize(modelMat * m_tan);
	vec3 Bitangent = normalize(modelMat * m_btan);
	vec3 Normal = normalize(normCoord);

	TBN = mat3(Tangent, Bitangent, Normal);

	fragPos = vec3(transform * vec4(aPos, 1.0));
}
