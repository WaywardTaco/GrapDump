
#include "RenderLine.hpp"
#include <iostream>

using namespace Engine;

void RenderLine::Update(double deltaTime){
    this->p1 = p1;
    this->p2 = p2;
}

void RenderLine::Render(Shader* shader, Camera* camera){

    glUseProgram(0);

    glm::vec4 d1 = camera->getProjection() * glm::vec4 (
        p1->x, p1->y, p1->z, 0.0f
    );
    glm::vec4 d2 = camera->getProjection() * glm::vec4 (
        p2->x, p2->y, p2->z, 0.0f
    );

     shader->use();
    glUseProgram(0);
    glBegin(GL_LINES);
    glColor3f(color.x, color.y, color.z);
    glVertex3f(d1.x, d1.y, d1.z);
    glVertex3f(d2.x, d2.y, d2.z);
    glEnd();
}
