
#version 330 core

// Texture Stuff
uniform sampler2D tex0; // where the texture gets assigned to
in vec2 texCoord; // Take it in from the vertex shader, same spelling and data type as vShader



out vec4 FragColor; // Returns a Color of indv pixels, required for frag shaders

void main() {

	// Range 0 to 1 :  R     G     B     A
	// FragColor = vec4 (0.7f, 0.7f, 0.5f, 1.f) ; // Color of the Fragment, simple single color

	// Adjusts the color of the fragment depending on coordinate
	FragColor = texture(tex0, texCoord);

}
