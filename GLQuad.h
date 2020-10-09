#pragma once
#include "GLTriangle.h"
#include <gtk/gtk.h>

/*!
    GLQuad
    ========

    This struct represents a Quad (Triangle 1 (ABD), Triangle 2 (CDB));
*/

typedef struct GLQuad {
    /*
        A-------------D
        |             |
        |             |
        |             |
        |             |
        |             |
        B-------------C
    */
    GLQuad();
    GLQuad(glm::vec3 leftTop, float side, glm::vec4 color = glm::vec4(1,1,1,1));
    
    void setCoordZ(float z);
    void updatePosition(glm::vec3 leftTop, float topSide, float leftSide = 0);
    void setTextCoord(int line, int col, int w = -1, int h = -1);
    void setTextCoord(glm::vec2);    
    void setColor(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d);
    void setColor(glm::vec4 color);
    void reset_textcoord(float val = -1);
    void reset_color();
    // first triangle -> A B D //
    GLTriangle T1;
    // second triangle -> C D B //
    GLTriangle T2;
}GLQuad;