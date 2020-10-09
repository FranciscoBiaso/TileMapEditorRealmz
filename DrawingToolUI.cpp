#include "DrawingToolUI.h"
#include <gdk/gdk.h>
#include "MapUI.h"

extern ui::MapUI* gMapUI;

// externs //
namespace GtkUserInterface { extern GtkBuilder* builder; }

ui::DrawingToolUI::DrawingToolUI()
{
    brush = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButtonBrush");
    eraser = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButtonEraser");
    zoomIn = gtk_builder_get_object(GtkUserInterface::builder, "gtkButtonZoomIn");
    zoomOut = gtk_builder_get_object(GtkUserInterface::builder, "gtkButtonZoomOut");
    zoomReset = gtk_builder_get_object(GtkUserInterface::builder, "gtkButtonZoomReset");
    viewDownStairs = gtk_builder_get_object(GtkUserInterface::builder, "gtkButtonViewDownStairs");
    buttonEye = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButtonEye");


    drawingMode = def::DrawingToolMode::DRAWING_NONE;

    g_signal_connect(brush, "toggled", G_CALLBACK(static_cb_signalGtkToggleButtonBrush), this);
    g_signal_connect(eraser, "toggled", G_CALLBACK(static_cb_signalGtkToggleButtonEraser), this);
    g_signal_connect(zoomIn, "clicked", G_CALLBACK(static_cb_signalGtkToggleButtonZoomIn), this);
    g_signal_connect(zoomOut, "clicked", G_CALLBACK(static_cb_signalGtkToggleButtonZoomOut), this);
    g_signal_connect(zoomReset, "clicked", G_CALLBACK(static_cb_signalGtkToggleButtonZoomReset), this);
    g_signal_connect(viewDownStairs, "clicked", G_CALLBACK(static_cb_signalGtkButtonViewDownStairs), this);
    g_signal_connect(buttonEye, "clicked", G_CALLBACK(static_cb_signalGtkToggleButtonEye), this);
}

def::DrawingToolMode ui::DrawingToolUI::getDrawingMode() const
{
    return drawingMode;
}


void ui::DrawingToolUI::static_cb_signalGtkToggleButtonEye(GtkToggleButton* togglebutton, gpointer user_data)
{
    reinterpret_cast<DrawingToolUI*>(user_data)->cb_signalGtkToggleButtonEye(togglebutton, user_data);
}

void ui::DrawingToolUI::static_cb_signalGtkButtonViewDownStairs(GtkButton* togglebutton, gpointer user_data)
{
    reinterpret_cast<DrawingToolUI*>(user_data)->cb_signalGtkButtonViewDownStairs(togglebutton, user_data);
}

void ui::DrawingToolUI::static_cb_signalGtkButtonReloadfiles(GtkButton* button, gpointer user_data)
{
    reinterpret_cast<DrawingToolUI*>(user_data)->cb_signalGtkButtonReloadfiles(button, user_data);
}

void ui::DrawingToolUI::static_cb_signalGtkToggleButtonZoomReset(GtkButton* togglebutton, gpointer user_data)
{
    reinterpret_cast<DrawingToolUI*>(user_data)->cb_signalGtkToggleButtonZoomReset(togglebutton, user_data);
}

void ui::DrawingToolUI::static_cb_signalGtkToggleButtonZoomIn(GtkButton* togglebutton, gpointer user_data)
{
    reinterpret_cast<DrawingToolUI*>(user_data)->cb_signalGtkToggleButtonZoomIn(togglebutton, user_data);
}

void ui::DrawingToolUI::static_cb_signalGtkToggleButtonZoomOut(GtkButton* togglebutton, gpointer user_data)
{
    reinterpret_cast<DrawingToolUI*>(user_data)->cb_signalGtkToggleButtonZoomOut(togglebutton, user_data);
}

void ui::DrawingToolUI::static_cb_signalGtkToggleButtonBrush(GtkToggleButton* togglebutton, gpointer user_data)
{
    reinterpret_cast<DrawingToolUI*>(user_data)->cb_signalGtkToggleButtonBrush(togglebutton, user_data);
}

void ui::DrawingToolUI::static_cb_signalGtkToggleButtonEraser(GtkToggleButton* togglebutton, gpointer user_data)
{
    reinterpret_cast<DrawingToolUI*>(user_data)->cb_signalGtkToggleButtonEraser(togglebutton, user_data);
}

void ui::DrawingToolUI::cb_signalGtkToggleButtonEye(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        drawingMode = def::DrawingToolMode::DRAWING_EYE;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(eraser), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(brush), FALSE);
    }
    else if (areAllToogleFalse())
    {
        drawingMode = def::DrawingToolMode::DRAWING_NONE;
    }
}

void ui::DrawingToolUI::cb_signalGtkToggleButtonBrush(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        drawingMode = def::DrawingToolMode::DRAWING_BRUSH;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(eraser), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttonEye), FALSE);
    }
    else if (areAllToogleFalse())
    {
        drawingMode = def::DrawingToolMode::DRAWING_NONE;
    }
}

void ui::DrawingToolUI::cb_signalGtkToggleButtonEraser(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        drawingMode = def::DrawingToolMode::DRAWING_ERASE;
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(brush), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttonEye), FALSE);
    }
    else if(areAllToogleFalse())
    {
        drawingMode = def::DrawingToolMode::DRAWING_NONE;
    }
}

void ui::DrawingToolUI::cb_signalGtkButtonViewDownStairs(GtkButton* button, gpointer user_data)
{  
    gMapUI->setCanSeeDownStairs(true);        
    gMapUI->forceRedraw();
}

void ui::DrawingToolUI::cb_signalGtkToggleButtonZoomReset(GtkButton* togglebutton, gpointer user_data)
{
    gMapUI->getGLScene()->resetZoom();
    gMapUI->forceRedraw();
}

void ui::DrawingToolUI::cb_signalGtkButtonReloadfiles(GtkButton* button, gpointer user_data)
{
    gMapUI->cleanAutoBorders();
    gMapUI->loadAutoBorderFromJson();
}

void ui::DrawingToolUI::cb_signalGtkToggleButtonZoomIn(GtkButton* togglebutton, gpointer user_data)
{
    gMapUI->getGLScene()->zoomIn();
    gMapUI->forceRedraw();
}

void ui::DrawingToolUI::cb_signalGtkToggleButtonZoomOut(GtkButton* togglebutton, gpointer user_data)
{
    gMapUI->getGLScene()->zoomOut();
    gMapUI->forceRedraw();
}

bool ui::DrawingToolUI::areAllToogleFalse()
{
    return !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(eraser)) &&
           !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(brush)) && 
           !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(buttonEye))
           ? true : false;
}

void ui::DrawingToolUI::setDrawingMode(def::DrawingToolMode drawingMode)
{
    this->drawingMode = drawingMode;
    if(drawingMode == def::DrawingToolMode::DRAWING_BRUSH)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(brush), TRUE);
    else if(drawingMode == def::DrawingToolMode::DRAWING_ERASE)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(eraser), TRUE);
    else if (drawingMode == def::DrawingToolMode::DRAWING_EYE)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttonEye), TRUE);
    else if (drawingMode == def::DrawingToolMode::DRAWING_NONE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(brush), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(eraser), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttonEye), FALSE);
    }
}

void ui::DrawingToolUI::setPreviousDrawingMode(def::DrawingToolMode drawingMode)
{
    this->drawingModePrevious = drawingMode;
}

def::DrawingToolMode ui::DrawingToolUI::gePrevioustDrawingMode() const
{
    return this->drawingModePrevious;
}

bool ui::DrawingToolUI::is_brush_enable()
{
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(brush));
}

bool ui::DrawingToolUI::is_eraser_enable()
{
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(eraser));
}