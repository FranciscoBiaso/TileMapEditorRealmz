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
    toggleButtonScript = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButtonScript");
    reloadFiles = gtk_builder_get_object(GtkUserInterface::builder, "gtkButtonReloadfiles");
    buttonAutoBorder = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButtonAutoBorder");
    buttonMagicWand = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButtonMagicWand");

    drawingMode = def::DrawingToolMode::DRAWING_NONE;

    g_signal_connect(brush, "toggled", G_CALLBACK(static_cb_signalGtkToggleButtonBrush), this);
    g_signal_connect(eraser, "toggled", G_CALLBACK(static_cb_signalGtkToggleButtonEraser), this);
    g_signal_connect(toggleButtonScript, "toggled", G_CALLBACK(static_cb_signalGtkToggleButtonScript), this);
    g_signal_connect(buttonEye, "toggled", G_CALLBACK(static_cb_signalGtkToggleButtonEye), this);
    g_signal_connect(zoomIn, "clicked", G_CALLBACK(static_cb_signalGtkToggleButtonZoomIn), this);
    g_signal_connect(zoomOut, "clicked", G_CALLBACK(static_cb_signalGtkToggleButtonZoomOut), this);
    g_signal_connect(zoomReset, "clicked", G_CALLBACK(static_cb_signalGtkToggleButtonZoomReset), this);
    g_signal_connect(viewDownStairs, "clicked", G_CALLBACK(static_cb_signalGtkButtonViewDownStairs), this);
    g_signal_connect(reloadFiles, "clicked", G_CALLBACK(static_cb_signalGtkButtonReloadfiles), this);

    gtk_widget_set_tooltip_text(GTK_WIDGET(brush), "It is the tool used to draw thing objects on the map.\n" 
                                                   "It is necessary to load a thing object from the stuff\n" 
                                                   "book on the left side of the screen to start drawing");
    gtk_widget_set_tooltip_text(GTK_WIDGET(eraser), "It is the tool used to erase things on the map.");
    gtk_widget_set_tooltip_text(GTK_WIDGET(zoomIn), "Brings the camera closer to the scene.");
    gtk_widget_set_tooltip_text(GTK_WIDGET(zoomOut), "Take the camera away from the scene.");
    gtk_widget_set_tooltip_text(GTK_WIDGET(reloadFiles), "Used to reload auto border and random tiles from \njson files. Will be detailed below.");
    gtk_widget_set_tooltip_text(GTK_WIDGET(viewDownStairs), "It generates transparency in the current floor, \nallowing you to see the floor below."
                                                     "To change the edited floor, hold the ctrl key and scroll with the mouse wheel.");
    gtk_widget_set_tooltip_text(GTK_WIDGET(buttonEye), "Hides the tile during the game, until the party (in exploration mode)\n"
                                                     "or character(in Combat mode) sees it or a script or ability reveals it.\n"
                                                     "Until then it will be  displayed as a black square, like an empty tile");
    gtk_widget_set_tooltip_text(GTK_WIDGET(toggleButtonScript), "Enables the insertion of scripts on the map.It must be done by activating\n"
                                                     "the shift key.Then, hold the mouse buttonand drag to define a rectangle.");    
    gtk_widget_set_tooltip_text(GTK_WIDGET(buttonAutoBorder), "Enables the use of auto border.");
    gtk_widget_set_tooltip_text(GTK_WIDGET(buttonMagicWand), "Enables the use of random tiles.");
    gtk_widget_set_tooltip_text(GTK_WIDGET(zoomReset), "Restart camera vision.");

}

def::DrawingToolMode ui::DrawingToolUI::getDrawingMode() const
{
    return drawingMode;
}

void ui::DrawingToolUI::static_cb_signalGtkToggleButtonScript(GtkToggleButton* togglebutton, gpointer user_data)
{
    reinterpret_cast<DrawingToolUI*>(user_data)->cb_signalGtkToggleButtonScript(togglebutton, user_data);
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

void ui::DrawingToolUI::static_cb_signalGtkButtonAutoBorder(GtkToggleButton* togglebutton, gpointer user_data)
{
    reinterpret_cast<DrawingToolUI*>(user_data)->cb_signalGtkButtonAutoBorder(togglebutton, user_data);
}

void ui::DrawingToolUI::static_cb_signalGtkButtonMagicWand(GtkToggleButton* togglebutton, gpointer user_data)
{
    reinterpret_cast<DrawingToolUI*>(user_data)->cb_signalGtkButtonMagicWand(togglebutton, user_data);
}

GtkToggleButton* ui::DrawingToolUI::getToggleButton(enum def::DrawingToolMode mode)
{
    switch (mode)
    {
    case def::DRAWING_NONE: return NULL;
        break; 
    case def::DRAWING_BRUSH: return GTK_TOGGLE_BUTTON(brush);
        break;
    case def::DRAWING_ERASE: return GTK_TOGGLE_BUTTON(eraser);
        break;
    case def::DRAWING_EYE: return GTK_TOGGLE_BUTTON(buttonEye);
        break;
    case def::DRAWING_SCRIPT: return GTK_TOGGLE_BUTTON(toggleButtonScript);
        break;
    case def::SELECTING_ERASE: return NULL;
        break;
    case def::SELECTING_BRUSH: return NULL;
        break;
    case def::SELECTING_SCRIPT: return NULL;
        break;
    default:
        break;
    }
    return NULL;
}

void ui::DrawingToolUI::disableAllToggleButtonsMinus(GtkToggleButton* togglebutton, enum def::DrawingToolMode minusMode)
{
    if(togglebutton != GTK_TOGGLE_BUTTON(buttonEye)) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(buttonEye), FALSE);
    if (togglebutton != GTK_TOGGLE_BUTTON(eraser)) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(eraser), FALSE);
    if (togglebutton != GTK_TOGGLE_BUTTON(brush)) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(brush), FALSE);
    if (togglebutton != GTK_TOGGLE_BUTTON(toggleButtonScript)) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggleButtonScript), FALSE);
    drawingMode = minusMode;
    gtk_toggle_button_set_active(togglebutton, TRUE);
}

void ui::DrawingToolUI::cb_signalGtkToggleButtonScript(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        disableAllToggleButtonsMinus(togglebutton, def::DrawingToolMode::DRAWING_SCRIPT);
    }
    else if (areAllToogleFalse())
    {
        drawingMode = def::DrawingToolMode::DRAWING_NONE;
    }
}
void ui::DrawingToolUI::cb_signalGtkToggleButtonEye(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        disableAllToggleButtonsMinus(togglebutton, def::DrawingToolMode::DRAWING_EYE);
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
        disableAllToggleButtonsMinus(togglebutton, def::DrawingToolMode::DRAWING_BRUSH);
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
        disableAllToggleButtonsMinus(togglebutton, def::DrawingToolMode::DRAWING_ERASE);
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
    gMapUI->cleanRandomTiles();
    gMapUI->loadAutoBorderFromJson();
    gMapUI->loadRandomTilesFromJson();
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

void ui::DrawingToolUI::cb_signalGtkButtonAutoBorder(GtkToggleButton* button, gpointer user_data)
{
}

void ui::DrawingToolUI::cb_signalGtkButtonMagicWand(GtkToggleButton* button, gpointer user_data)
{
}


bool ui::DrawingToolUI::areAllToogleFalse()
{
    return !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(eraser)) &&
           !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(brush)) && 
           !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(buttonEye)) &&
           !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggleButtonScript))
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

bool ui::DrawingToolUI::is_script_enable()
{
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggleButtonScript));
}

bool ui::DrawingToolUI::is_autoborder_enable()
{
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(buttonAutoBorder));
}

bool ui::DrawingToolUI::is_magicwand_enable()
{
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(buttonMagicWand));
}