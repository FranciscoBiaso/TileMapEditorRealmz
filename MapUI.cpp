#include "MapUI.h"
#include "MapResources.h"
#include "AuxUI.h"
#include "DrawingFunctions.h"
#include "DrawingToolUI.h"

extern data::MapResources* gResources;
extern ui::AuxUI* gAuxUI;
extern ui::DrawingToolUI* gDrawingToolUI;

namespace GtkUserInterface { extern GtkBuilder* builder; }

ui::MapUI::MapUI(std::string name, int width, int height) : Map(name,width,height)
{
    gtkMapViewPort = gtk_builder_get_object(GtkUserInterface::builder, "gtkMapViewPort");

    drawingArea = gtk_drawing_area_new();

    gtk_widget_add_events(drawingArea, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(drawingArea, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(drawingArea, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(drawingArea, GDK_LEAVE_NOTIFY_MASK);
    gtk_widget_add_events(drawingArea, GDK_ENTER_NOTIFY_MASK);

    gtk_widget_set_size_request(GTK_WIDGET(drawingArea), REALMZ_GRID_SIZE * MAP_SCENE_WIDTH , REALMZ_GRID_SIZE * MAP_SCENE_HEIGHT );
    gtk_container_add(GTK_CONTAINER(gtkMapViewPort), drawingArea);
        
    g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(static_cb_draw_callback), this);
    g_signal_connect(G_OBJECT(drawingArea), "button-press-event", G_CALLBACK(static_cb_clickNotify), this);
    g_signal_connect(G_OBJECT(drawingArea), "button-release-event", G_CALLBACK(static_cb_clickNotify), this);
    g_signal_connect(G_OBJECT(drawingArea), "motion-notify-event", G_CALLBACK(static_cb_MotionNotify), this);
    g_signal_connect(G_OBJECT(drawingArea), "enter-notify-event", G_CALLBACK(static_cb_onEnter), this);
    g_signal_connect(G_OBJECT(drawingArea), "leave-notify-event", G_CALLBACK(static_cb_onLeave), this);

    g_signal_connect(gtkMapViewPort, "size-allocate", G_CALLBACK(static_my_getsize), this);

    thingIsSelected = false;
    drawingModes = DRAWING_EMPTY;
}

gboolean ui::MapUI::cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    drawGrid(cr, viewWidth, viewHeight, REALMZ_GRID_SIZE);

    drawMap(cr);

    GdkRGBA HLSColor; HLSColor.red = 0; HLSColor.green = 0; HLSColor.blue = 0.85; HLSColor.alpha = 0.25;
    graphics::drawSquare(cr, mousePosition.getX() * REALMZ_GRID_SIZE, mousePosition.getY() * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);

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

gboolean ui::MapUI::static_cb_onEnter(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    return reinterpret_cast<MapUI*>(user_data)->cb_onEnter(widget, event, user_data);
}

gboolean ui::MapUI::static_cb_onLeave(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    return reinterpret_cast<MapUI*>(user_data)->cb_onLeave(widget, event, user_data);
}


gboolean ui::MapUI::cb_MotionNotify(GtkWidget* widget, GdkEventMotion* e, gpointer user_data)
{
    mousePosition.setX((int)(e->x / REALMZ_GRID_SIZE) );
    mousePosition.setY((int)(e->y / REALMZ_GRID_SIZE) );

    if (drawingModes == DRAWING_PEN_SELECTED &&
        mousePositionPrevious != mousePosition) // we only add new item if mouse square changes //
    {
        mousePositionPrevious = mousePosition;
        addThingMapUI();
    }

    gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
    return TRUE;
}

gboolean ui::MapUI::cb_clickNotify(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    if (event->type == GDK_BUTTON_PRESS)
    {
        addThingMapUI(); // starting adding //
        mousePositionPrevious = mousePosition;
        drawingModes = DRAWING_PEN_SELECTED;
    }
    else if (event->type == GDK_BUTTON_RELEASE)
    {
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

void ui::MapUI::addThingMapUI()
{
    // mouse x is col, y is row //
    if (thingIsSelected)
    {
        addThing(drawObj, mousePosition.getY(), mousePosition.getX(), 0);
        gAuxUI->printMsg("Thing " + drawObj.getName() + "[" + drawObj.getType() + "]" + " added!");
        gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
    }
    else
    {
        gAuxUI->printMsg("You need To select a Thing before drawn!");
    }
}

void ui::MapUI::selectCursor()
{
    switch(gDrawingToolUI->getDrawingMode())
    {
    case def::DrawingMode::DRAWING_NONE:
        gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(drawingArea)), gdk_cursor_new_for_display(gdk_display_get_default(), GDK_ARROW));
        break;
    case def::DrawingMode::DRAWING_BRUSH:
        gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(drawingArea)), gdk_cursor_new_for_display(gdk_display_get_default(), GDK_PENCIL));
        break;
    case def::DrawingMode::DRAWING_ERASE:
        gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(drawingArea)), gdk_cursor_new_for_display(gdk_display_get_default(), GDK_SPRAYCAN));
        break;
    }
}

gboolean ui::MapUI::cb_onEnter(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    selectCursor();
    return TRUE;
}

gboolean ui::MapUI::cb_onLeave(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    // outside mapUI we use the default cursor //
    gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(drawingArea)), gdk_cursor_new_for_display(gdk_display_get_default(), GDK_ARROW));
    return TRUE;
}

void ui::MapUI::deletAllThingsFromTheMap(std::string thingName)
{
  deletAllThings(thingName);
}

void ui::MapUI::forceRedraw()
{
  gtk_widget_queue_draw(GTK_WIDGET(drawingArea)); // force redraw map //
}