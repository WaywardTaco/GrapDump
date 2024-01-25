
#version 330 core

// Gets the data at Attrib in Index 0
// Converts it and stores it into a vec3 called aPos

layout(location = 0) in vec3 aPos;

// Declare a variable to hold the data that we plan to pass to the vertecies
uniform mat4 transform ;

void main () {
	// gl_Position is predefined variable that stores the final positional data of the the point
	gl_Position = transform * vec4 (aPos, 1.0); // Turns the vec3 into a vec4 for Layering, fixing Z-Fighting, & applies matrix mult for transform
}
