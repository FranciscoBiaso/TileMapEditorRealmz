#include "AuxUI.h"


// class static members //
GObject* ui::AuxUI::gtkEntryThingSearch = nullptr;
GObject* ui::AuxUI::gtkStatusBar = nullptr;

// externs //
namespace GtkUserInterface { extern GtkBuilder* builder; }
extern data::MapResources* gResources;
extern ui::StuffBookUI* gStuffBook;

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
    searchThingByName(std::string(gtk_entry_get_text(GTK_ENTRY(gtkEntryThingSearch))));
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

bool ui::AuxUI::searchThingByName(std::string name)
{  
    std::string entryText = gtk_entry_get_text(GTK_ENTRY(gtkEntryThingSearch));
    
    gint row_count = 0; // count rows, not used  //
    gboolean valid; // check error //
    GtkTreeIter iter; // our node to iterate through the model //
    GtkTreeModel* model = gtk_tree_view_get_model(gStuffBook->getTreeViewStuffBook()); // grab model //

    /* Get the first iter in the list */
    valid = gtk_tree_model_get_iter_first(model, &iter);

    bool founded = false; // start as not founded //
    while (valid && !founded) // while iterating and not founded //
    {
        GtkTreeIter iterChildren;
        gboolean validChildren;

        validChildren = gtk_tree_model_iter_children(model, &iterChildren, &iter);
        while (validChildren && !founded) // iterate through children //
        {
            gchar* str_data;
            gtk_tree_model_get(model, &iterChildren, 0, &str_data, -1); // grab data from model //
            
            if (std::string(str_data) == entryText) // compare //
            {                                
                GtkTreePath* path = gtk_tree_model_get_path(model, &iter);                
                gtk_tree_view_expand_row(gStuffBook->getTreeViewStuffBook(), path, false); // expand row //
                GtkTreePath* pathCell = gtk_tree_model_get_path(model, &iterChildren);
                gtk_tree_selection_select_iter(gtk_tree_view_get_selection(gStuffBook->getTreeViewStuffBook()), &iterChildren); // select item //
                gtk_tree_view_scroll_to_cell(gStuffBook->getTreeViewStuffBook(), pathCell, NULL, false, 1.0, 1.0); // adjust scroll window //                
                gtk_tree_path_free(path);                
                founded = true; // we founded //
            }
            g_free(str_data); // clean //

            validChildren = gtk_tree_model_iter_next(model, &iterChildren); // jump to next data //
        }
        
        valid = gtk_tree_model_iter_next(model, &iter); // jump to next data //
        row_count++; // increment row //
    }

    // print msg //
    if (!founded)
    {
        std::string statusMsg = "Thing [" + entryText + "] not founded!";
        gtk_statusbar_push(GTK_STATUSBAR(gtkStatusBar), 0, statusMsg.c_str());
    }

    return founded;
}