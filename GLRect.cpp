#include "GLRect.h"
#include "MapResources.h"

extern data::MapResources* gResources;

GLRect::GLRect(){}

GLRect::GLRect(glm::vec3 leftTop, glm::vec3 bottomRight, glm::vec4 color)
{
    glm::vec3 A = leftTop;
    glm::vec3 B = leftTop - glm::vec3(0, glm::abs(leftTop.y - bottomRight.y), 0);
    glm::vec3 C = bottomRight;
    glm::vec3 D = leftTop + glm::vec3(glm::abs(leftTop.x - bottomRight.x), 0, 0);

    T1 = GLTriangle(A, B, D);
    T2 = GLTriangle(B, C, D);
    setColor(color);
}

GLRect::GLRect(glm::vec3 leftTop, float side, glm::vec4 color)
{
    // Opengl Coords //
    /*
               U
      (0,0) ---------> 1
        |
       V|
        |
        v
        1
    */
    updatePosition(leftTop, side);
    setColor(color);
}


void GLRect::setColor(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d)
{
    this->T1.setColor(a, b, d);
    this->T2.setColor(b, c, d);
}

void GLRect::setTextCoord(int u, int v, int w, int h)
{
    if (w == -1 && h == -1)
    {
        w = gdk_pixbuf_get_width(gResources->getImgPack().getTextureAtlas()->getPixelbuf());
        h = gdk_pixbuf_get_height(gResources->getImgPack().getTextureAtlas()->getPixelbuf());
    }

    float x_left = (u * REALMZ_GRID_SIZE) / (float)w;
    float x_right = (u * REALMZ_GRID_SIZE + REALMZ_GRID_SIZE) / (float)w;
    float y_top = (v * REALMZ_GRID_SIZE) / (float)h;
    float y_bot = (v * REALMZ_GRID_SIZE + REALMZ_GRID_SIZE) / (float)h;

    T1.setTextCoord(glm::vec2(x_left, y_top), glm::vec2(x_left, y_bot), glm::vec2(x_right, y_top));
    T2.setTextCoord(glm::vec2(x_left, y_bot), glm::vec2(x_right, y_bot), glm::vec2(x_right, y_top));
}

void  GLRect::setCoordZ(float z)
{
    T1.setCoordZ(z);
    T2.setCoordZ(z);
}

void GLRect::setTextCoord(glm::vec2 coords)
{
    setTextCoord(coords.x, coords.y);
}

void GLRect::updatePosition(glm::vec3 leftTop, float topSide, float leftSide)
{
    if (leftSide == 0)
        leftSide = topSide;
    glm::vec3 A = leftTop;
    glm::vec3 B = leftTop + glm::vec3(0, -leftSide, 0);
    glm::vec3 C = leftTop + glm::vec3(+topSide, -leftSide, 0);
    glm::vec3 D = leftTop + glm::vec3(topSide, 0, 0);

    T1 = GLTriangle(A, B, D);
    T2 = GLTriangle(B, C, D);
}

void GLRect::reset_textcoord(float val)
{
    T1.A_text_coord = glm::vec2(val, val);
    T1.B_text_coord = glm::vec2(val, val);
    T1.C_text_coord = glm::vec2(val, val);
    T2.A_text_coord = glm::vec2(val, val);
    T2.B_text_coord = glm::vec2(val, val);
    T2.C_text_coord = glm::vec2(val, val);
}

void GLRect::setColor(glm::vec4 color)
{
    setColor(color, color, color, color);
}

void GLRect::reset_color()
{
    setColor(glm::vec4(0, 0, 0, 0));
}
