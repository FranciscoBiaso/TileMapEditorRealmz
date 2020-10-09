#include "DrawingFunctions.h"

void graphics::drawSquare(cairo_t* cr, int x, int y, int w, int h, GdkRGBA color)
{
    cairo_rectangle(cr, x, y, w, h);
    cairo_set_source_rgba(cr, color.red, color.green, color.blue, color.alpha);
    cairo_fill(cr);
}

void graphics::get_LeftTop_rightbot(glm::vec2 A, glm::vec2 B, glm::vec2& leftTop, glm::vec2& rightBot)
{
    if (A.y > B.y && A.x < B.x)
    {
        leftTop = A;
        rightBot = B;
    }
    if (A.y < B.y && A.x < B.x)
    {
        leftTop = glm::vec2(A.x, B.y);
        rightBot = glm::vec2(B.x, A.y);
    }
    if (A.y < B.y && A.x > B.x)
    {
        leftTop = B;
        rightBot = A;
    }
    if (A.y > B.y && A.x > B.x)
    {
        leftTop = glm::vec2(B.x, A.y);
        rightBot = glm::vec2(A.x, B.y);
    }

    if (A.x == B.x || A.y == B.y)
    {
        leftTop = A;
        rightBot = B;
    }
}