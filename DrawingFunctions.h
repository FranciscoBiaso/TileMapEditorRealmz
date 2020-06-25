#pragma once
#include <gdk/gdk.h>

namespace graphics{
    void drawSquare(cairo_t* cr, int x, int y, int w, int h, GdkRGBA color);
}
