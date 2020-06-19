#include "MapUI.h"
#include "MapResources.h"

extern data::MapResources* gResources;

namespace GtkUserInterface { extern GtkBuilder* builder; }

ui::MapUI::MapUI(std::string name, int width, int height) : Map(name,width,height)
{
    gtkMapFrame = gtk_builder_get_object(GtkUserInterface::builder, "gtkMapFrame");

    drawingArea = gtk_drawing_area_new();

    gtk_widget_set_size_request(GTK_WIDGET(drawingArea), REALMZ_GRID_SIZE * MAP_SCENE_WIDTH, REALMZ_GRID_SIZE * MAP_SCENE_HEIGHT);
    gtk_container_add(GTK_CONTAINER(gtkMapFrame), drawingArea);
        
    g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(static_cb_draw_callback), this);
}

gboolean ui::MapUI::cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    drawMap(cr);

    return FALSE;
}

void ui::MapUI::static_cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data) {
    reinterpret_cast<MapUI*>(data)->cb_draw_callback(widget, cr, data);
}