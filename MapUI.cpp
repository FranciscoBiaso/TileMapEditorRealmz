#include "MapUI.h"
#include "MapResources.h"
#include "AuxUI.h"

extern data::MapResources* gResources;
extern ui::AuxUI* gAuxUI;

namespace GtkUserInterface { extern GtkBuilder* builder; }

ui::MapUI::MapUI(std::string name, int width, int height) : Map(name,width,height)
{
    gtkMapViewPort = gtk_builder_get_object(GtkUserInterface::builder, "gtkMapViewPort");

    drawingArea = gtk_drawing_area_new();

    gtk_widget_add_events(drawingArea, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(drawingArea, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(drawingArea, GDK_BUTTON_RELEASE_MASK);

    gtk_widget_set_size_request(GTK_WIDGET(drawingArea), REALMZ_GRID_SIZE * MAP_SCENE_WIDTH , REALMZ_GRID_SIZE * MAP_SCENE_HEIGHT );
    gtk_container_add(GTK_CONTAINER(gtkMapViewPort), drawingArea);
        
    g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(static_cb_draw_callback), this);
    g_signal_connect(G_OBJECT(drawingArea), "button-press-event", G_CALLBACK(static_cb_clickNotify), this);
    g_signal_connect(G_OBJECT(drawingArea), "button-release-event", G_CALLBACK(static_cb_clickNotify), this);
    g_signal_connect(G_OBJECT(drawingArea), "motion-notify-event", G_CALLBACK(static_cb_MotionNotify), this);

    g_signal_connect(gtkMapViewPort, "size-allocate", G_CALLBACK(static_my_getsize), this);

    thingIsSelected = false;
    drawingModes = DRAWING_EMPTY;
}

gboolean ui::MapUI::cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    drawGrid(cr, viewWidth, viewHeight, REALMZ_GRID_SIZE);

    drawMap(cr);

    return FALSE;
}

void ui::MapUI::static_cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data) 
{
    reinterpret_cast<MapUI*>(data)->cb_draw_callback(widget, cr, data);
}

gboolean ui::MapUI::static_cb_clickNotify(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    return reinterpret_cast<MapUI*>(user_data)->cb_clickNotify(widget, event, user_data);
}

gboolean ui::MapUI::static_cb_MotionNotify(GtkWidget* widget, GdkEventMotion* event, gpointer user_data)
{
    return reinterpret_cast<MapUI*>(user_data)->cb_MotionNotify(widget, event, user_data);
}

gboolean ui::MapUI::cb_MotionNotify(GtkWidget* widget, GdkEventMotion* e, gpointer user_data)
{
    mousePosition.setX((int)(e->x / REALMZ_GRID_SIZE) );
    mousePosition.setY((int)(e->y / REALMZ_GRID_SIZE) );

    if (drawingModes == DRAWING_PEN_SELECTED)
    {
        drawThing();
    }
    return TRUE;
}

gboolean ui::MapUI::cb_clickNotify(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    if (event->type == GDK_BUTTON_PRESS)
    {
        std::cout << "press" << std::endl;
        drawingModes = DRAWING_PEN_SELECTED;
        drawThing();
    }
    else if (event->type == GDK_BUTTON_RELEASE)
    {
        std::cout << "release" << std::endl;
        drawingModes = DRAWING_EMPTY;
    }

    return TRUE;
}

void ui::MapUI::setDrawThingObj(data::Thing thing)
{
    drawObj = thing;

    thingIsSelected = true;
}

void ui::MapUI::drawGrid(cairo_t* cr, int w, int h, int gridSize)
{
    if (w == -1 || h == -1) return;

    cairo_set_line_width(cr, 1.2);
    //double dashs[2] = { 1.0, 1.0 };
    //cairo_set_dash(cr, dashs, 1, 0);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
    cairo_set_source_rgba(cr, 0.95, 0.95, 0.1, 1);

    for (int l = 0; l <= h; l+=REALMZ_GRID_SIZE)
    {
        // draw vertical line //
        cairo_move_to(cr, 0, l);
        cairo_line_to(cr, w, l);

        for (int c = 0; c <= w; c+=REALMZ_GRID_SIZE)
        {
            cairo_move_to(cr, c, 0);
            cairo_line_to(cr, c, h);
        }

    }
    cairo_stroke(cr);
    cairo_fill(cr);
}

void ui::MapUI::drawThing()
{
    // mouse x is col, y is row //
    if (thingIsSelected)
    {
        this->addThing(drawObj, mousePosition.getY(), mousePosition.getX(), 0);
        gAuxUI->printMsg("Thing " + drawObj.getName() + "[" + drawObj.getType() + "]" + " added!");
        gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
    }
    else
    {
        gAuxUI->printMsg("You need To select a Thing before drawn!");
    }

}