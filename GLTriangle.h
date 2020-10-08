#pragma once

#include <glm/glm.hpp>

/*!
    GLTriangle
    ========

    This struct represents a triangle (vertex, color, texture);
*/

typedef struct GLTriangle {
    GLTriangle();
    GLTriangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec2 A_text = glm::vec2(0, 0), glm::vec2 B_text = glm::vec2(0, 0), glm::vec2 C_text = glm::vec2(0, 0));
    void setCoordZ(float z);
    void setColor(glm::vec4 a, glm::vec4 b, glm::vec4 c);
    void setTextCoord(glm::vec2 a, glm::vec2 b, glm::vec2 c);

    // The order is important as it will be sent to opengl draw functions //
    glm::vec3 A; 
    glm::vec4 colorA;
    glm::vec2 A_text_coord;
    glm::vec3 B;
    glm::vec4 colorB;
    glm::vec2 B_text_coord;
    glm::vec3 C;
    glm::vec4 colorC;
    glm::vec2 C_text_coord;
}GLTriangle;