#include <gtk/gtk.h>
#include "ThingCreatorTool.h"
#include "GraphicsTool.h"
#include "MapResources.h"
#include "AuxUI.h"
#include "ImgPackUI.h"
#include "MapUI.h"
#include "DrawingToolUI.h"
#include "CtrlMap.h"
#include "AppLoaderSettings.h"
#include "MapInfoLoader.h"
#include "SceneScripts.h"
#include "ScriptUI.h"

#include <Windows.h>

#ifdef TME_DEBUG
#include "DebugTextureAtlas.h"
#endif // TME_DEBUG

// global variables //---------------------//
namespace GtkUserInterface { GtkBuilder* builder;}
data::MapResources* gResources = nullptr;
ui::StuffBookUI* gStuffBook = nullptr;
ui::ScriptUI* gScriptUI = nullptr;
ui::ImgPackUI * gImgPackUI = nullptr;
ui::AuxUI* gAuxUI = nullptr;
ui::MapUI* gMapUI = nullptr;
ui::ThingCreatorTool* gThingCreatorTool = nullptr;
ui::GraphicsTool* gGraphicsTool = nullptr;
ui::DrawingToolUI* gDrawingToolUI = nullptr;
ctrl::CtrlMap* ctrlMap = nullptr;
Scripts::ContainerSceneScript* gSceneScripts = nullptr;
AppLoaderSettings gAppLoaderSettings;
MapInfoLoader gMapInfoLoader;
GtkWidget* window; // Main window
//-----------------------------------------//

#ifdef TME_DEBUG
DebugTextureAtlas* debugTextureAtlas = nullptr;
#endif

bool ctrlPressed;
bool isSavingMap;

static gboolean fill(gpointer  user_data)
{
    GtkWidget* progress_bar = (GtkWidget*)(user_data);

    /*Get the current progress*/
    gdouble fraction;
    fraction = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progress_bar));

    /*Increase the bar by 10% each time this function is called*/
    fraction += 0.1;

    /*Fill in the bar with the new fraction*/
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), fraction);

    /*Ensures that the fraction stays below 1.0*/
    if (!isSavingMap)
    {
        //gtk_window_close((GtkWindow*)p_window);
        return TRUE;
    }
    return FALSE;
}

int create_window() {
    GtkWidget* p_main_box = NULL;

    //Create window
    //p_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //gtk_window_set_title(GTK_WINDOW(p_window), "Saving map!");
    //gtk_window_set_default_size(GTK_WINDOW(p_window), 320, 200);

    GtkWidget* progress_bar = gtk_progress_bar_new();
   // gtk_container_add(GTK_CONTAINER(p_window), progress_bar);
    g_timeout_add(500, fill, GTK_PROGRESS_BAR(progress_bar));

    //gtk_widget_show_all(p_window);
    return true;
}

static gboolean cb_clickNotify(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    if (event->type == GDK_KEY_PRESS)
    {
        if (event->key.keyval == GDK_KEY_Control_L)
        {
            ctrlPressed = true;
        }

        if (ctrlPressed && (event->key.keyval == GDK_KEY_s || event->key.keyval == GDK_KEY_S))
        {
            //create_window();
            gAuxUI->printMsg("Saving map data!");
            gResources->getImgPack().getTextureAtlas()->saveAtlasAsImg();
            gResources->getImgPack().getTextureAtlas()->saveAtlasInfoAsJson();
            gResources->getImgPack().saveImgPackAsJsonFile();
            gResources->saveStuffBook();
            gMapUI->saveMap();
            gScriptUI->saveScriptsAsJson();
        }
    }
   
    if (event->type == GDK_KEY_RELEASE)
    {
        if (event->key.keyval == GDK_KEY_Control_L)
        {
            ctrlPressed = false;
        }
    }

    return FALSE;
}

int main(int argc, char** argv)
{
    //FreeConsole();
    // Gtk lib initialization //---------------------//
    gtk_init(&argc, &argv);
    //---------------------//

    // Gtk Builder //---------------------//
    GtkUserInterface::builder = gtk_builder_new();
    gtk_builder_add_from_file(GtkUserInterface::builder, "builder.ui", NULL);
    gtk_builder_connect_signals(GtkUserInterface::builder, NULL);
    //---------------------//

    gAuxUI = new ui::AuxUI();
    gAppLoaderSettings.load();
    gMapInfoLoader.load();

    // Resources //---------------------//
    gResources = new data::MapResources();
    //---------------------//

    // user interface // ---------------------//
    gSceneScripts = new Scripts::ContainerSceneScript();

    //gSceneScripts->addScript(glm::vec2(0,0), 3 * REALMZ_GRID_SIZE, gAppLoaderSettings.getMapLevels() - 1, "", "", glm::vec4(1, 0.5, 0.25, 0.35));
    //gSceneScripts->addScript(glm::vec2(5 * REALMZ_GRID_SIZE, - 5 * REALMZ_GRID_SIZE), 5 * REALMZ_GRID_SIZE, gAppLoaderSettings.getMapLevels() - 1, "", "", glm::vec4(0.5, 1, 0.25, 0.35));


    gStuffBook = new ui::StuffBookUI();
    gScriptUI = new ui::ScriptUI();
    gImgPackUI = new ui::ImgPackUI();
    gThingCreatorTool = new ui::ThingCreatorTool();
    gGraphicsTool = new ui::GraphicsTool();
    gMapUI = new ui::MapUI(gAppLoaderSettings.getMapNameToLoad(), gMapInfoLoader.getMapWidth(), gMapInfoLoader.getMapHeight(), gMapInfoLoader.getMapLevels());
    gDrawingToolUI = new ui::DrawingToolUI();
    //---------------------//

    gGraphicsTool->loadImgPackFromJson();
    gResources->loadStuffBookFromJson();
    gStuffBook->updateTree(); // update tree view //
    gMapUI->loadAutoBorderFromJson();
    gMapUI->loadRandomTilesFromJson();
    gMapUI->loadMapFromJson();
    gScriptUI->loadScriptsFromJson();

    // Controller // ---------------------//
    ctrlMap = new ctrl::CtrlMap();
    //---------------------//


#ifdef TME_DEBUG
    debugTextureAtlas = new DebugTextureAtlas(gResources->getImgPack().getTextureAtlas()->getPixelbuf());
#endif

    window = GTK_WIDGET(gtk_builder_get_object(GtkUserInterface::builder, "window"));  
    
      
    gtk_widget_add_events(window, GDK_ALL_EVENTS_MASK);
    g_signal_connect(G_OBJECT(window), "button-release-event", G_CALLBACK(cb_clickNotify), NULL);
    g_signal_connect(G_OBJECT(window), "button-press-event", G_CALLBACK(cb_clickNotify), NULL);
    g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(cb_clickNotify), NULL);
    g_signal_connect(G_OBJECT(window), "key-release-event", G_CALLBACK(cb_clickNotify), NULL);    
    gtk_window_maximize((GtkWindow*)window);
    gtk_widget_show_all(window);


    if (gAppLoaderSettings.getEditorType() == "map_maker")
    {
        gtk_widget_hide(gThingCreatorTool->getParentFrame());
        gtk_widget_hide(gGraphicsTool->getParentFrame());
    }

    // activ user interface if needed //
    gScriptUI->activeButtons();
    
    gtk_main();


    delete gResources;
    delete gStuffBook;
    delete gScriptUI;
    delete gImgPackUI;
    delete gAuxUI;
    delete gThingCreatorTool;
    delete gGraphicsTool;
    delete gMapUI;
    delete gDrawingToolUI;
    delete ctrlMap;
    delete gSceneScripts;
    return 0;
}

