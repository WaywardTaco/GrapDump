
#include "RenderLine.hpp"

using namespace Engine;

void RenderLine::Update(double deltaTime){
    this->p1 = p1;
    this->p2 = p2;
}

void RenderLine::Render(Shader* shader, Camera* camera, bool isOrtho){

    glUseProgram(0);

    float mag1 = p1->magnitude();
    float mag2 = p2->magnitude();
    
    glm::vec4 d1 = camera->getProjection() * camera->getViewMat() * glm::vec4 (
        glm::vec3(p1->x, p1->y, p1->z), 1.0f
    );
    glm::vec4 d2 = camera->getProjection() * camera->getViewMat() * glm::vec4 (
        glm::vec3(p2->x, p2->y, p2->z), 1.0f
    );

    float max1 = glm::max(glm::max(d1.x, d1.y), d1.z);
    float max2 = glm::max(glm::max(d2.x, d2.y), d2.z);

    if(isOrtho){
        max1 = 1;
        max2 = 1;
    }

    d1 = glm::vec4(d1.x / max1, d1.y / max1, d1.y / max1, d1.a);
    d2 = glm::vec4(d2.x / max2, d2.y / max2, d2.y / max2, d2.a);
    
    // glm::normalize(d1);
    // glm::normalize(d2);
    
    // float mag = sqrt(d1.x * d1.x + d1.y * d1.y + d1.z * d1.z);

    // d1.x /= mag;
    // d1.y /= mag;
    // d1.z /= mag;

    // mag = sqrt(d2.x * d2.x + d2.y * d2.y + d2.z * d2.z);

    // d2.x /= mag;
    // d2.y /= mag;
    // d2.z /= mag;

    shader->use();
    glUseProgram(0);
    glBegin(GL_LINES);
    glColor3f(color.x, color.y, color.z);
    glVertex3f(d1.x, d1.y, d1.z);
    glVertex3f(d2.x, d2.y, d2.z);
    glEnd();
}
