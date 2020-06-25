#include <gtk/gtk.h>
#include "ThingCreatorTool.h"
#include "GraphicsTool.h"
#include "MapResources.h"
#include "AuxUI.h"
#include "ImgPackUI.h"
#include "MapUI.h"
#include "DrawingToolUI.h"

#ifdef TME_DEBUG
#include "DebugTextureAtlas.h"
#endif // TME_DEBUG

// global variables //---------------------//
namespace GtkUserInterface { GtkBuilder* builder;}
data::MapResources* gResources = nullptr;
ui::StuffBookUI* gStuffBook = nullptr;
ui::ImgPackUI * gImgPackUI = nullptr;
ui::AuxUI* gAuxUI = nullptr;
ui::MapUI* gMapUI = nullptr;
ui::ThingCreatorTool* gThingCreatorTool = nullptr;
ui::GraphicsTool* gGraphicsTool = nullptr;
ui::DrawingToolUI* gDrawingToolUI = nullptr;
//-----------------------------------------//

#ifdef TME_DEBUG
DebugTextureAtlas* debugTextureAtlas = nullptr;
#endif

int main(int argc, char** argv)
{
    // Gtk lib initialization //---------------------//
    gtk_init(&argc, &argv);
    //---------------------//

    // Gtk Builder //---------------------//
    GtkUserInterface::builder = gtk_builder_new();
    gtk_builder_add_from_file(GtkUserInterface::builder, "builder.ui", NULL);
    gtk_builder_connect_signals(GtkUserInterface::builder, NULL);
    //---------------------//

    // Resources //---------------------//
    gResources = new data::MapResources();
    //---------------------//

    // user interface // ---------------------//
    gStuffBook = new ui::StuffBookUI();
    gImgPackUI = new ui::ImgPackUI();
    gAuxUI = new ui::AuxUI();
    gThingCreatorTool = new ui::ThingCreatorTool();
    gGraphicsTool = new ui::GraphicsTool();
    gMapUI = new ui::MapUI("map", 20, 20);
    gDrawingToolUI = new ui::DrawingToolUI();
    //---------------------//


#ifdef TME_DEBUG
    debugTextureAtlas = new DebugTextureAtlas(gResources->getImgPack().getTextureAtlas()->getPixelbuf());
#endif


    GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(GtkUserInterface::builder, "window"));
    gtk_widget_show_all(window);
    gtk_main();

    delete gResources;
    delete gStuffBook;
    delete gImgPackUI;
    delete gAuxUI;
    delete gThingCreatorTool;
    delete gGraphicsTool;
    delete gMapUI;
    delete gDrawingToolUI;
    return 0;
}