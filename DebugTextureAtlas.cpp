#include "DebugTextureAtlas.h"


GtkWidget* DebugTextureAtlas::drawingArea;
cairo_surface_t* DebugTextureAtlas::surface = nullptr;
GObject* DebugTextureAtlas::gtkMapFrame;
GtkWidget* DebugTextureAtlas::p_window = nullptr;

namespace GtkUserInterface { extern GtkBuilder* builder; }

DebugTextureAtlas::DebugTextureAtlas(GdkPixbuf * pixlBuff)
{
    p_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(p_window), "Debug window");
    gtk_window_set_default_size(GTK_WINDOW(p_window), 500, 300);

	static GObject* gtkViewportImgSrc;
	gtkMapFrame = gtk_builder_get_object(GtkUserInterface::builder, "gtkMapFrame"); 
    
    drawingArea = gtk_drawing_area_new();
    //GtkWidget* parent = gtk_widget_get_parent(GTK_WIDGET(gtkMapFrame));
    gtk_container_add(GTK_CONTAINER(gtkMapFrame), p_window);
    gtk_container_add(GTK_CONTAINER(p_window), drawingArea);
    g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(cb_draw_callback), NULL);
    surface = gdk_cairo_surface_create_from_pixbuf(pixlBuff, 0, NULL);
}

gboolean DebugTextureAtlas::cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    if (surface != nullptr)
    {
        cairo_set_source_surface(cr, surface, 0, 0);
        cairo_paint(cr);
    }
    return TRUE;
}