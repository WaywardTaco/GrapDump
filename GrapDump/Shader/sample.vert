
#version 330 core

out mat3 TBN;
out vec2 texCoord; // output to the frag shader
out vec3 normCoord;
out vec3 fragPos;

// Start Vertex Transform Stuff
// Gets the data at Attrib in Index 0
// Converts it and stores it into a vec3 called aPos
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertexNormal;
// UV Data is at index 2
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 m_tan;
layout(location = 4) in vec3 m_btan;

// Declare a variable to hold the data that we plan to pass to the vertecies
uniform mat4 transform ;

// Projection Matrix
uniform mat4 projection ;

// Camera View Matrix
uniform mat4 view ;

void main () {
	// gl_Position is predefined variable that stores the final positional data of the the point
	gl_Position = projection * view * transform * vec4 (aPos, 1.0); // Turns the vec3 into a vec4 for Layering, fixing Z-Fighting, & applies matrix mult for transform

	// sets aTex texture to texCoord which is passed to frag shader
	texCoord = aTex;

	mat3 modelMat = mat3(transpose(inverse(transform))); // Normal matrix

	normCoord = modelMat * vertexNormal ; // Normal Info

	vec3 Tangent = normalize(modelMat * m_tan);
	vec3 Bitangent = normalize(modelMat * m_btan);
	vec3 Normal = normalize(normCoord);

	TBN = mat3(Tangent, Bitangent, Normal);

	fragPos = vec3(transform * vec4(aPos, 1.0));
}
