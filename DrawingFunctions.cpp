#include "DrawingFunctions.h"

void graphics::drawSquare(cairo_t* cr, int x, int y, int w, int h, GdkRGBA color)
{
    cairo_rectangle(cr, x, y, w, h);
    cairo_set_source_rgba(cr, color.red, color.green, color.blue, color.alpha);
    cairo_fill(cr);
}