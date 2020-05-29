#include "AuxUI.h"

// class static members //
GObject* ui::AuxUI::gtkEntryThingSearch = nullptr;
GObject* ui::AuxUI::gtkStatusBar = nullptr;

// externs //
namespace GtkUserInterface { extern GtkBuilder* builder; }
extern data::MapResources* gResources;
extern ui::StuffBookUI* gStuffBook;

// constructor //
ui::AuxUI::AuxUI()
{
    // loading interfaces from xml //
    gtkEntryThingSearch = gtk_builder_get_object(GtkUserInterface::builder, "gtkEntryThingSearch");
    gtkStatusBar = gtk_builder_get_object(GtkUserInterface::builder, "gtkStatusBar");

    // callbacks //
    g_signal_connect(gtkEntryThingSearch, "activate", G_CALLBACK(cb_onActive), NULL);
    g_signal_connect(gtkEntryThingSearch, "focus-in-event", G_CALLBACK(cb_onFocusInEvent), NULL);
    g_signal_connect(gtkEntryThingSearch, "focus-out-event", G_CALLBACK(cb_onFocusOutEvent), NULL);
}


void ui::AuxUI::cb_onActive(GtkWidget* widget, gpointer data)
{
    ui::AuxUI::cb_doSearchThing(NULL, NULL);
}

void ui::AuxUI::cb_onFocusInEvent(GtkWidget* widget, gpointer data)
{
    GtkEntryBuffer* gtkEntryBuffer = gtk_entry_get_buffer(GTK_ENTRY(gtkEntryThingSearch));
    gtk_entry_buffer_delete_text(gtkEntryBuffer, 0, gtk_entry_buffer_get_length(gtkEntryBuffer));    
}

void ui::AuxUI::cb_onFocusOutEvent(GtkWidget* widget, gpointer data)
{
    GtkEntryBuffer* gtkEntryBuffer = gtk_entry_get_buffer(GTK_ENTRY(gtkEntryThingSearch));
    std::string buffer = "...";
    gtk_entry_buffer_set_text(gtkEntryBuffer, buffer.c_str(), buffer.size());
}

void ui::AuxUI::cb_doSearchThing(GtkWidget* widget, gpointer data)
{  
    std::string entryText = gtk_entry_get_text(GTK_ENTRY(gtkEntryThingSearch));
    if (data != NULL)
        entryText = (char *)data;
    
    gint row_count = 0;
    gboolean valid;
    GtkTreeIter iter;
    GtkTreeModel* model = gtk_tree_view_get_model(gStuffBook->getTreeViewStuffBook());

    /* Get the first iter in the list */
    valid = gtk_tree_model_get_iter_first(model, &iter);

    bool founded = false;
    while (valid && !founded)
    {
        GtkTreeIter iterChildren;
        gboolean validChildren;


        validChildren = gtk_tree_model_iter_children(model, &iterChildren, &iter);
        while (validChildren && !founded)
        {
            gchar* str_data;
            gtk_tree_model_get(model, &iterChildren, 0, &str_data, -1);
            
            if (std::string(str_data) == entryText)
            {                                
                GtkTreePath* path = gtk_tree_model_get_path(model, &iter);
                //gtk_tree_selection_select_path(gtk_tree_view_get_selection(GTK_TREE_VIEW(gStuffBook->gtkTreeViewStuffBook)), path);
                gtk_tree_view_expand_row(gStuffBook->getTreeViewStuffBook(), path, false);


                GtkTreePath* pathCell = gtk_tree_model_get_path(model, &iterChildren);

                gtk_tree_selection_select_iter(gtk_tree_view_get_selection(gStuffBook->getTreeViewStuffBook()), &iterChildren);

                gtk_tree_view_scroll_to_cell(gStuffBook->getTreeViewStuffBook(), pathCell, NULL, false, 1.0, 1.0);
                
                gtk_tree_path_free(path);                

                // gtk_widget_show(GTK_WIDGET(gStuffBook->gtkTreeViewStuffBook));
                //gtk_tree_view_set_focus

                founded = true;
            }
            g_free(str_data);
            validChildren = gtk_tree_model_iter_next(model, &iterChildren);
        }
        

        valid = gtk_tree_model_iter_next(model, &iter);
        row_count++;
    }

    if (!founded)
    {
        std::string statusMsg = "Thing [" + entryText + "] not founded!";
        gtk_statusbar_push(GTK_STATUSBAR(gtkStatusBar), 0, statusMsg.c_str());
    }
}