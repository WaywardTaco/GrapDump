
#version 330 core

// Start Texture Stuff
// UV Data is at index 2
layout(location = 2) in vec2 aTex;
out vec2 texCoord; // output to the frag shader

out vec3 normCoord;
out vec3 fragPos;

// Start Vertex Transform Stuff
// Gets the data at Attrib in Index 0
// Converts it and stores it into a vec3 called aPos
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertexNormal;

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

	normCoord = mat3(
		transpose(inverse(transform)) // Normal Matrix
	) * vertexNormal ; // Normal Info

	fragPos = vec3(transform * vec4(aPos, 1.0));
}
