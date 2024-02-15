
#version 330 core

out vec4 FragColor; // Returns a Color of indv pixels, required for frag shaders

void main() {

	// Range 0 to 1 :  R     G     B     A
	FragColor = vec4 (0.7f, 0.7f, 0.5f, 1.f) ; // Color of the Fragment, simple single color

}
