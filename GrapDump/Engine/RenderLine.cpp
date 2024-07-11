
#include "RenderLine.hpp"

using namespace Engine;

void RenderLine::Update(Vector3 p1, Vector3 p2, glm::mat4 projection){
    this->p1 = p1;
    this->p2 = p2;
    this->projectionMat = projection;
}

void RenderLine::Draw(){
    glUseProgram(0);

    glm::vec4 d1 = this->projectionMat * glm::vec4 (
        p1.x, p1.y, p1.z, 1.0f
    );
    glm::vec4 d2 = this->projectionMat * glm::vec4 (
        p2.x, p2.y, p2.z, 1.0f
    );

    glBegin(GL_LINES);
    glVertex3f(d1.x, d1.y, d1.z);
    glVertex3f(d2.x, d2.y, d2.z);
    glEnd();
}
