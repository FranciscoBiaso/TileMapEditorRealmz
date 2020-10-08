#include "GLTriangle.h"

GLTriangle::GLTriangle(){}

GLTriangle::GLTriangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec2 A_text, glm::vec2 B_text, glm::vec2 C_text)
{
    this->A = A;
    this->B = B;
    this->C = C;
    setTextCoord(A_text, B_text, C_text);
}

void GLTriangle::setColor(glm::vec4 a, glm::vec4 b, glm::vec4 c)
{
    this->colorA = a;
    this->colorB = b;
    this->colorC = c;
}

void  GLTriangle::setCoordZ(float z)
{
    this->A.z = z;
    this->B.z = z;
    this->C.z = z;
}

void GLTriangle::setTextCoord(glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
    this->A_text_coord = a;
    this->B_text_coord = b;
    this->C_text_coord = c;
}