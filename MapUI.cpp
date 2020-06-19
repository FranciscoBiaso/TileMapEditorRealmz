#include "MapUI.h"
#include "MapResources.h"

extern data::MapResources* gResources;

namespace GtkUserInterface { extern GtkBuilder* builder; }

ui::MapUI::MapUI(std::string name, int width, int height) : Map(name,width,height)
{
    gtkMapFrame = gtk_builder_get_object(GtkUserInterface::builder, "gtkMapFrame");

    pixelbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE);
    gdk_pixbuf_fill(pixelbuf, 0x22222211); // clean buffer //

    drawingArea = gtk_drawing_area_new();

    gtk_widget_set_size_request(GTK_WIDGET(drawingArea), REALMZ_GRID_SIZE * MAP_SCENE_WIDTH, REALMZ_GRID_SIZE * MAP_SCENE_HEIGHT);
    gtk_container_add(GTK_CONTAINER(gtkMapFrame), drawingArea);
        
    g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(static_cb_draw_callback), this);
}

gboolean ui::MapUI::cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data)
{

    gdk_cairo_set_source_pixbuf(cr, pixelbuf, 0, 0);


    gdk_pixbuf_fill(pixelbuf, 0xffffffff); // clean buffer //
    gdk_cairo_set_source_pixbuf(cr, pixelbuf, 0, 0);
    cairo_paint(cr);


    gdk_pixbuf_fill(pixelbuf, 0xff000022); // clean buffer //
    gdk_cairo_set_source_pixbuf(cr, pixelbuf, 0, 0);
    cairo_paint(cr);

    gdk_pixbuf_fill(pixelbuf, 0x00ff0022); // clean buffer //
    gdk_cairo_set_source_pixbuf(cr, pixelbuf, 0, 0);
    cairo_paint(cr);

    gdk_pixbuf_fill(pixelbuf, 0xff000022); // clean buffer //
    gdk_cairo_set_source_pixbuf(cr, pixelbuf, 32, 0);
    cairo_paint(cr);

    gdk_pixbuf_fill(pixelbuf, 0x00ff0022); // clean buffer //
    gdk_cairo_set_source_pixbuf(cr, pixelbuf, 64, 0);
    cairo_paint(cr);
    drawMap(cr);


    
    
    return FALSE;
}