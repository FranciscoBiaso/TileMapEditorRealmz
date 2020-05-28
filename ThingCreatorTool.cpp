#include "ThingCreatorTool.h"

Thing ui::ThingCreatorTool::thing("", "");
GObject* ui::ThingCreatorTool::gtkEntryThingName = nullptr;
GObject* ui::ThingCreatorTool::gtkEntryThingImg = nullptr;
GObject* ui::ThingCreatorTool::gtkTreeViewThingType = nullptr;
GObject* ui::ThingCreatorTool::gtkTreeViewThingObj = nullptr;
GObject* ui::ThingCreatorTool::gtkButtonCreateThing = nullptr;


namespace GtkUserInterface {
    extern GtkBuilder* builder;
}

extern data::MapResources*gResources;
extern ui::StuffBookUI* gStuffBook;
extern ui::AuxUI* gAuxUI;

ui::ThingCreatorTool::ThingCreatorTool()
{
    /* load interface [xml file .ui]*/
    
    gtkEntryThingName = gtk_builder_get_object(GtkUserInterface::builder, "gtkEntryThingName");    
    gtkEntryThingImg = gtk_builder_get_object(GtkUserInterface::builder, "gtkEntryThingImg");

    gtkTreeViewThingType = gtk_builder_get_object(GtkUserInterface::builder, "gtkTreeViewThingType");
    gtkTreeViewThingObj = gtk_builder_get_object(GtkUserInterface::builder, "gtkTreeViewThingObj");
    gtkButtonCreateThing = gtk_builder_get_object(GtkUserInterface::builder, "gtkButtonCreateThing");

    // creating models 
    createTreeViewThingType();
    createTreeViewThingObj();

    // update functions 
    g_signal_connect(gtkButtonCreateThing, "clicked", G_CALLBACK(createThing), NULL);        
    //g_signal_connect(gtkButtonAddThingName, "clicked", G_CALLBACK(updateThingName), NULL);
    g_signal_connect(gtkTreeViewThingType, "row-activated", G_CALLBACK(updateThingType), NULL);
    g_signal_connect(gtkEntryThingName, "changed", G_CALLBACK(onGtkEntryThingNameChanged), NULL);
    
    updateTreeThingType();
    updateTreeThingObj();
}


void ui::ThingCreatorTool::onGtkEntryThingNameChanged(GtkWidget* widget, gpointer data)
{
    updateThingName();
}

/* update functions */

void ui::ThingCreatorTool::updateThingName()
{
    thing.setName(gtk_entry_get_text(GTK_ENTRY(gtkEntryThingName)));

    updateTreeThingObj();
}

void ui::ThingCreatorTool::updateThingType(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data)
{
    gchar* str_data1;

    GtkTreeIter iter;
    GtkTreeModel* model = gtk_tree_view_get_model(tree_view);

    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(model, &iter, 0, &str_data1, -1);
        // Here the variables int_data and str_data should be filled with
        // relevant data
        thing.setType(str_data1);        
        updateTreeThingObj();
    }
}

void ui::ThingCreatorTool::createTreeViewThingType()
{
    GtkCellRenderer* renderer;
    GtkTreeViewColumn* col;
    
    col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col, "ThingTypes");
    gtk_tree_view_append_column(GTK_TREE_VIEW(gtkTreeViewThingType), col);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", 0);

    gtk_tree_view_set_show_expanders(GTK_TREE_VIEW(gtkTreeViewThingType), true);
    gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(gtkTreeViewThingType), true);
}

void ui::ThingCreatorTool::createTreeViewThingObj()
{

    GtkCellRenderer* renderer;
    GtkCellRenderer* renderer2;
    GtkTreeViewColumn* col;
    GtkTreeViewColumn* col2;

    col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col, "ThingObj");
    gtk_tree_view_column_set_title(col, "atribute");
    col2 = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col2, "value");
    gtk_tree_view_append_column(GTK_TREE_VIEW(gtkTreeViewThingObj), col);
    gtk_tree_view_append_column(GTK_TREE_VIEW(gtkTreeViewThingObj), col2);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", 0);

    renderer2 = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col2, renderer2, TRUE);
    gtk_tree_view_column_add_attribute(col2, renderer2, "text", 1);
}

void ui::ThingCreatorTool::updateTreeThingObj()
{
    GtkTreeModel* model = fillTreeThingObj();
    gtk_tree_view_set_model(GTK_TREE_VIEW(gtkTreeViewThingObj), model);    
    g_object_unref(model);
}

void ui::ThingCreatorTool::updateTreeThingType()
{
    GtkTreeModel* model = fillTreeThingType();
    gtk_tree_view_set_model(GTK_TREE_VIEW(gtkTreeViewThingType), model);
    g_object_unref(model);
}

GtkTreeModel* ui::ThingCreatorTool::fillTreeThingType()
{
    GtkTreeStore* treestore;
    GtkTreeIter toplevel, child;

    treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING);

    Json::Value obj = gResources->getThingTypesJson();
    
    const Json::Value& types = obj["types"]; 
    
    for (auto i = 0; i < types.size(); i++) {
        std::string name = types[i]["name"].asString();
        int layer = types[i]["layer"].asUInt();
        const Json::Value& _class = types[i]["class"];
        
        if (_class.size() != 0) {
            gtk_tree_store_append(treestore, &toplevel, NULL);
            gtk_tree_store_set(treestore, &toplevel, COLUMN, name.c_str(), -1);

            for (auto j = 0; j < _class.size(); j++) {                
                gtk_tree_store_append(treestore, &child, &toplevel);
                gtk_tree_store_set(treestore, &child,COLUMN, _class[j]["name"].asString().c_str(),-1);
            }
        }
        else {
            gtk_tree_store_append(treestore, &toplevel, NULL);
            gtk_tree_store_set(treestore, &toplevel,COLUMN, name.c_str(),-1);
        }
    }

    return GTK_TREE_MODEL(treestore);
}

GtkTreeModel* ui::ThingCreatorTool::fillTreeThingObj()
{
    GtkTreeStore* treestore;
    GtkTreeIter toplevel;

    treestore = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
   
    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel, 0, "name:", 1, thing.getName().c_str(), -1);
    
    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel, 0, "thingType:", 1, thing.getType().c_str(), -1);
    
    return GTK_TREE_MODEL(treestore);
}

void ui::ThingCreatorTool::createThing(GtkWidget* widget, gpointer data)
{    
    gResources->addThing(thing); // add thing into stuffBook //
    gStuffBook->updateTree();
    gAuxUI->doSearchThing(NULL, (gpointer*)thing.getName().c_str());
}
