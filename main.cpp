#include <gtk/gtk.h>
#include "ThingCreatorTool.h"
#include "GraphicsTool.h"
#include "MapResources.h"
#include "AuxUI.h"

// global variables //---------------------//
namespace GtkUserInterface {
    GtkBuilder* builder;
}
data::MapResources* gResources;
ui::StuffBookUI* gStuffBook;
ui::AuxUI* gAuxUI;
ui::ThingCreatorTool* gThingCreatorTool;
ui::GraphicsTool* gGraphicsTool;
//-----------------------------------------//

int main(int argc, char** argv)
{
    // Gtk lib initialization //---------------------//
    GtkWidget* window;
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
    gAuxUI = new ui::AuxUI();
    gThingCreatorTool = new ui::ThingCreatorTool();
    gGraphicsTool = new ui::GraphicsTool();
    //---------------------//

    window = GTK_WIDGET(gtk_builder_get_object(GtkUserInterface::builder,"window"));
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}