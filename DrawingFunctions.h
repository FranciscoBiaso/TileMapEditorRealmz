#pragma once
#include <gdk/gdk.h>
#include <glm/glm.hpp>

namespace graphics{
    void drawSquare(cairo_t* cr, int x, int y, int w, int h, GdkRGBA color);
    void get_LeftTop_rightbot(glm::vec2 A, glm::vec2 B, glm::vec2& leftTop, glm::vec2& rightBot);
    
}
