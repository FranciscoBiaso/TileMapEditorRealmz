#include "ScriptUI.h"
#include "SceneScripts.h"
#include "MapUI.h"

GObject* ui::ScriptUI::gtkTreeViewScript = nullptr;
GObject* ui::ScriptUI::gtkScrolledWindowScript = nullptr;
GObject* ui::ScriptUI::gtkToggleButtonScriptText = nullptr; 
GObject* ui::ScriptUI::gtkToggleButtonScriptRects = nullptr;
GObject* ui::ScriptUI::gtkEntryScriptSearch = nullptr;

namespace GtkUserInterface { extern GtkBuilder* builder; }

extern Scripts::ContainerSceneScript* gSceneScripts;
extern ui::MapUI* gMapUI;

ui::ScriptUI::ScriptUI()
{
    gtkTreeViewScript = gtk_builder_get_object(GtkUserInterface::builder, "gtkTreeViewScript");
    gtkScrolledWindowScript = gtk_builder_get_object(GtkUserInterface::builder, "gtkScrolledWindowScript");
    gtkToggleButtonScriptText = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButtonScriptText");
    gtkToggleButtonScriptRects = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButtonScriptRects");
    gtkEntryScriptSearch = gtk_builder_get_object(GtkUserInterface::builder, "gtkEntryScriptSearch");


    gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(gtkTreeViewScript), true);

   // g_signal_connect(gtkTreeViewStuffBook, "key-press-event", G_CALLBACK(static_cb_removeThing), this);

    createTreeView();
    updateTree();

    //g_signal_connect(G_OBJECT(gtkTreeViewStuffBook), "row-activated", G_CALLBACK(static_cb_selectThing), this);
    
    g_signal_connect(gtkToggleButtonScriptText, "toggled", G_CALLBACK(static_cb_signalGtkToggleButtonScriptText), this);
    g_signal_connect(gtkToggleButtonScriptRects, "toggled", G_CALLBACK(static_cb_signalGtkToggleButtonScriptRects), this);
    g_signal_connect(gtkTreeViewScript, "key-press-event", G_CALLBACK(static_cb_removeImgObj), this);
    g_signal_connect(gtkEntryScriptSearch, "activate", G_CALLBACK(static_cb_onActive), this);
    g_signal_connect(gtkTreeViewScript, "row-activated", G_CALLBACK(static_cb_rowActive), this);
    g_signal_connect(gtkEntryScriptSearch, "focus-in-event", G_CALLBACK(cb_onFocusInEvent), this);
}

gboolean ui::ScriptUI::cb_onFocusInEvent(GtkWidget* widget, GdkEvent* event, gpointer   user_data)
{
    GtkEntryBuffer* gtkEntryBuffer = gtk_entry_get_buffer(GTK_ENTRY(gtkEntryScriptSearch));
    gtk_entry_buffer_delete_text(gtkEntryBuffer, 0, gtk_entry_buffer_get_length(gtkEntryBuffer));
    return FALSE;
}

void ui::ScriptUI::static_cb_rowActive(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data)
{
    reinterpret_cast<ScriptUI*>(user_data)->cb_rowActive(tree_view, path, column, user_data);
}

void ui::ScriptUI::static_cb_onActive(GtkWidget* widget, gpointer user_data)
{
    reinterpret_cast<ScriptUI*>(user_data)->cb_onActive(widget, user_data);
}

gboolean ui::ScriptUI::static_cb_removeImgObj(GtkWidget* widget, GdkEventKey* event, gpointer   user_data)
{
    reinterpret_cast<ScriptUI*>(user_data)->cb_removeImgObj(widget, event, user_data);
    return TRUE;
}

void ui::ScriptUI::static_cb_signalGtkToggleButtonScriptRects(GtkToggleButton* togglebutton, gpointer user_data)
{
    reinterpret_cast<ScriptUI*>(user_data)->cb_signalGtkToggleButtonScriptRects(togglebutton, user_data);
}

void ui::ScriptUI::static_cb_signalGtkToggleButtonScriptText(GtkToggleButton* togglebutton, gpointer user_data)
{
    reinterpret_cast<ScriptUI*>(user_data)->cb_signalGtkToggleButtonScriptText(togglebutton, user_data);
}

void ui::ScriptUI::static_cb_clicked(GtkCellRendererText* renderer, char* path, char* new_text, gpointer user_data){

    reinterpret_cast<ScriptUI*>(user_data)->clicked(renderer, path, new_text, user_data);
}

void ui::ScriptUI::cb_onActive(GtkWidget* widget, gpointer user_data)
{
    std::string ID = std::string(gtk_entry_get_text(GTK_ENTRY(gtkEntryScriptSearch)));

    glm::vec4 toSelectColor = gSceneScripts->getScripts()[ID].rectColor;
    gSceneScripts->updateRectColor(gSceneScripts->getSelectedScriptID(), toSelectColor);
    gSceneScripts->setSelectedScriptID(ID);
    searchScriptByID(ID);
    gSceneScripts->updateRectColor(ID, glm::vec4(0, 0, 0.95, 0.25));
    gMapUI->forceRedraw();
}

void ui::ScriptUI::cb_rowActive(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data)
{
    GtkTreeSelection* gtkTreeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtkTreeViewScript)); // grab tree selection from mouse click //
    GtkTreeModel* model = nullptr;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(gtkTreeSelection, &model, &iter)) // if true //
    {
        char* scriptID;
        gtk_tree_model_get(model, &iter, 0, &scriptID, -1);
        glm::vec4 toSelectColor = gSceneScripts->getScripts()[std::string(scriptID)].rectColor;
        gSceneScripts->updateRectColor(gSceneScripts->getSelectedScriptID(), toSelectColor);

        gSceneScripts->setSelectedScriptID(std::string(scriptID));
        
        gSceneScripts->updateRectColor(std::string(scriptID), glm::vec4(0,0,0.95,0.25));
        gMapUI->forceRedraw();
    }
}

gboolean ui::ScriptUI::cb_removeImgObj(GtkWidget* widget, GdkEventKey* event, gpointer user_data)
{
    // is delete key ? if no, do nothing //
    if (event->keyval != GDK_KEY_Delete)
    {
        //gtk_statusbar_push(gAuxUI->getStatusBar(), 0, std::string("Press delete key to remove a ImgObj!").c_str());
        return TRUE;
    }

    // commun variables //    
    GtkTreeSelection* gtkTreeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget)); // grab tree selection from mouse click //
    GtkTreeModel* model = nullptr;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(gtkTreeSelection, &model, &iter)) // if true //
    {
        gchar* name;
        gtk_tree_model_get(model, &iter, 0, &name, -1);

        gSceneScripts->delScript(std::string(name));

        // rebuild all scripts from scracth //
        // copy all scripts //
        std::map<std::string, Scripts::SceneScript> mapScripts = gSceneScripts->getScripts();
        gSceneScripts->cleanScripts(); // clean container //
        gSceneScripts->cleanScriptRects(); // clean opengl quads //
        // reinsert all into container //
        for (auto it = mapScripts.begin(); it != mapScripts.end(); it++)
            gSceneScripts->addScript(it->second);
      
        // update view //
        updateTree();

        gMapUI->forceRedraw();
    }

    return TRUE;
}

void ui::ScriptUI::cb_signalGtkToggleButtonScriptText(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gSceneScripts->setShowScriptsTexts(true);
    }
    else
    {
        gSceneScripts->setShowScriptsTexts(false);
    }
    gMapUI->forceRedraw();
}

void ui::ScriptUI::cb_signalGtkToggleButtonScriptRects(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gSceneScripts->setShowScriptsRects(true);
    }
    else
    {
        gSceneScripts->setShowScriptsRects(false);
    }
    gMapUI->forceRedraw();
}

void ui::ScriptUI::clicked(GtkCellRendererText* renderer, char* path, char* new_text, gpointer user_data)
{
    GtkTreeSelection* gtkTreeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtkTreeViewScript)); // grab tree selection from mouse click //
    GtkTreeModel* model = nullptr;
    GtkTreeIter iter;


    auto mapScripts = gSceneScripts->getScripts();
    if (gtk_tree_selection_get_selected(gtkTreeSelection, &model, &iter)) // if true //
    {
        char * scriptID;
        gtk_tree_model_get(model, &iter, 0, &scriptID, -1);
        gSceneScripts->getScripts()[std::string(scriptID)].scriptToLoad = new_text;
        updateTree();
        gMapUI->forceRedraw();
    }
}

void ui::ScriptUI::createTreeView()
{
    GtkCellRenderer* renderer;
    GtkTreeViewColumn* col1;
    GtkTreeViewColumn* col2;

    // col 1 ID //
    col1 = gtk_tree_view_column_new();// create a new col //
    gtk_tree_view_column_set_sizing(col1, GTK_TREE_VIEW_COLUMN_FIXED); // set col as fixed //
    gtk_tree_view_column_set_title(col1, "ID"); // title, not used //
    gtk_tree_view_append_column(GTK_TREE_VIEW(gtkTreeViewScript), col1); // append col into tree view //

    renderer = gtk_cell_renderer_text_new(); // adjust how text is drawn using object properties //
    gtk_tree_view_column_pack_start(col1, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col1, renderer, "text", 0);

    // col 2 ID //
    col2 = gtk_tree_view_column_new();// create a new col //
    gtk_tree_view_column_set_sizing(col2, GTK_TREE_VIEW_COLUMN_FIXED); // set col as fixed //
    gtk_tree_view_column_set_title(col2, "script"); // title, not used //
    gtk_tree_view_append_column(GTK_TREE_VIEW(gtkTreeViewScript), col2); // append col into tree view //

    renderer = gtk_cell_renderer_text_new(); // adjust how text is drawn using object properties //        
    g_object_set(renderer, "editable", TRUE, NULL);
    gtk_tree_view_column_pack_start(col2, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col2, renderer, "text", 1);
    g_signal_connect(renderer, "edited", G_CALLBACK(static_cb_clicked), this);
    
    //gtk_tree_view_column_set_clickable(col2, true);
    
    gtk_tree_view_set_show_expanders(GTK_TREE_VIEW(gtkTreeViewScript), true); // show expanders //
    gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(gtkTreeViewScript), true); // tree lines view //

}

void ui::ScriptUI::updateTree()
{
    GtkTreeModel* model = fillTree(); // fil model //
    gtk_tree_view_set_model(GTK_TREE_VIEW(gtkTreeViewScript), model); // set //
    g_object_unref(model);   // unref //
}

GtkTreeModel* ui::ScriptUI::fillTree()
{
    GtkTreeStore* treestore;
    GtkTreeIter toplevel;

    treestore = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING); // type of store //
    auto mapType = gSceneScripts->getScripts();

    for (auto it = mapType.begin(); it != mapType.end(); it++) // iterate through scripts //
    {
        gtk_tree_store_append(treestore, &toplevel, NULL); // top level //
        gtk_tree_store_set(treestore, &toplevel, 0, it->first.c_str(),1, it->second.scriptToLoad.c_str(), -1); // set type //
        
        /*
        GtkTreeIter child;
        auto map = it->second; // access the sub dicionary [things] //
        for (auto it_j = map.begin(); it_j != map.end(); it_j++) // iterate through each thing into this type //
        {
            gtk_tree_store_append(treestore, &child, &toplevel); // add child //
            gtk_tree_store_set(treestore, &child, 0, it_j->first.c_str(), -1); // set name //
        }
        */
    }

    return GTK_TREE_MODEL(treestore);
}

void ui::ScriptUI::activeButtons()
{
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButtonScriptText), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButtonScriptRects), TRUE);
}

void ui::ScriptUI::searchScriptByID(std::string ID) 
{
    gint row_count = 0; // count rows, not used  //
    gboolean valid; // check error //
    GtkTreeIter iter; // our node to iterate through the model //
    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(gtkTreeViewScript)); // grab model //

    /* Get the first iter in the list */
    valid = gtk_tree_model_get_iter_first(model, &iter);

    bool founded = false; // start as not founded //
    gchar* str_data;
    GtkTreeIter iterChildren;
    gboolean validChildren;

    while (valid && !founded) // while iterating and not founded //
    {
        gtk_tree_model_get(model, &iter, 0, &str_data, -1); // grab data from model, check root //
        
        if (std::string(str_data) != ID)
        {
            valid = gtk_tree_model_iter_next(model, &iter); // jump to next data //
            row_count++; // increment row //            
        }
        else 
        {
            gtk_tree_selection_select_iter(gtk_tree_view_get_selection(GTK_TREE_VIEW(gtkTreeViewScript)), &iter); // select item //
            GtkTreePath* path = gtk_tree_model_get_path(model, &iter);
            gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(gtkTreeViewScript), path, NULL, false, 1.0, 1.0); // adjust scroll window //                
            gtk_tree_path_free(path);
            founded = true;
        }       
    }   
}
/*
glm::vec2 leftTopPoint; //ok
glm::vec2 rightBotPoint;
glm::vec4 borderColor;
glm::vec4 rectColor;
float borderSize;
float floor;
float zRect;
float zBorder;
float zText;
std::string name;
std::string scriptToLoad;
glm::vec2 offsetText;
*/
void ui::ScriptUI::saveScriptsAsJson()
{
    Json::Value jsonValue;
    auto mapScripts = gSceneScripts->getScripts();


    for (auto it = mapScripts.begin(); it != mapScripts.end(); it++)
    {
        std::string scriptName = it->second.scriptToLoad;
        glm::vec2 leftTopPoint = it->second.leftTopPoint;
        glm::vec2 rightBotPoint = it->second.rightBotPoint;
        glm::vec2 offsetText = it->second.offsetText;
        glm::vec4 borderColor = it->second.borderColor;
        glm::vec4 rectColor = it->second.rectColor;
        float borderSize = it->second.borderSize;
        float floor = it->second.floor;
        float zRect = it->second.zRect;
        float zBorder = it->second.zBorder;
        float zText = it->second.zText;

        jsonValue[it->first]["scriptToLoad"] = scriptName;
        jsonValue[it->first]["leftTopPoint"]["x"] = leftTopPoint.x;
        jsonValue[it->first]["leftTopPoint"]["y"] = leftTopPoint.y;
        jsonValue[it->first]["rightBotPoint"]["x"] = rightBotPoint.x;
        jsonValue[it->first]["rightBotPoint"]["y"] = rightBotPoint.y;
        jsonValue[it->first]["offsetText"]["x"] = offsetText.x;
        jsonValue[it->first]["offsetText"]["y"] = offsetText.y;
        jsonValue[it->first]["borderColor"]["r"] = borderColor.r;
        jsonValue[it->first]["borderColor"]["g"] = borderColor.g;
        jsonValue[it->first]["borderColor"]["b"] = borderColor.b;
        jsonValue[it->first]["borderColor"]["a"] = borderColor.a;
        jsonValue[it->first]["rectColor"]["r"] = rectColor.r;
        jsonValue[it->first]["rectColor"]["g"] = rectColor.g;
        jsonValue[it->first]["rectColor"]["b"] = rectColor.b;
        jsonValue[it->first]["rectColor"]["a"] = rectColor.a;
        jsonValue[it->first]["borderSize"] = borderSize;        
        jsonValue[it->first]["floor"] = gMapUI->getLevels() - 1 - floor;
    }

    Json::StreamWriterBuilder builder;

    builder["commentStyle"] = "None";
    builder["indentation"] = "   ";
    std::unique_ptr<Json::StreamWriter> writer(
        builder.newStreamWriter());

    // Make a new JSON document for the configuration. Preserve original comments.


    std::ofstream ofs("resources//scripts.json", std::ofstream::out);// file to read //
    writer->write(jsonValue, &ofs);

    ofs.close();
}

void ui::ScriptUI::loadScriptsFromJson()
{
    std::ifstream ifs("resources//scripts.json");// file to read //
    Json::CharReaderBuilder rbuilder;	// reader //
    std::string errs; // to check errors //
    Json::Value jsonObj;
    data::Thing thing;
    Json::parseFromStream(rbuilder, ifs, &jsonObj, &errs); // parser // 
    if (!jsonObj.isNull()) // loading img pack //
    {
        for (auto it = jsonObj.begin(); it != jsonObj.end(); it++)
        {
            std::string scriptID = it.name();
            glm::vec2 leftTopPoint = glm::vec2((*it)["leftTopPoint"]["x"].asFloat(), (*it)["leftTopPoint"]["y"].asFloat());
            glm::vec2 rightBotPoint = glm::vec2((*it)["rightBotPoint"]["x"].asFloat(), (*it)["rightBotPoint"]["y"].asFloat());
            glm::vec2 offsetText = glm::vec2((*it)["offsetText"]["x"].asFloat(), (*it)["offsetText"]["y"].asFloat());
            glm::vec4 borderColor = glm::vec4((*it)["borderColor"]["r"].asFloat(), (*it)["borderColor"]["g"].asFloat(), (*it)["borderColor"]["b"].asFloat(), (*it)["borderColor"]["a"].asFloat());
            glm::vec4 rectColor = glm::vec4((*it)["rectColor"]["r"].asFloat(), (*it)["rectColor"]["g"].asFloat(), (*it)["rectColor"]["b"].asFloat(), (*it)["rectColor"]["a"].asFloat());
            float borderSize = (*it)["borderSize"].asFloat();
            float floor = (*it)["floor"].asFloat() + gMapUI->getLevels() - 1;

            gSceneScripts->addScript(Scripts::SceneScript(leftTopPoint, rightBotPoint, floor, rectColor, borderColor, borderSize, offsetText));
        }
    }
    updateTree();
}