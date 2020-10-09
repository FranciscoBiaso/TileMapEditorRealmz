#include "StuffBookUI.h"
#include "MapUI.h"
#include "DrawingToolUI.h"

GObject* ui::StuffBookUI::gtkTreeViewStuffBook = nullptr;
GObject* ui::StuffBookUI::gtkScrolledWindowStuffbook = nullptr;

namespace GtkUserInterface { extern GtkBuilder* builder;}

extern data::MapResources* gResources;
extern ui::AuxUI* gAuxUI;
extern ui::MapUI* gMapUI;
extern ui::DrawingToolUI* gDrawingToolUI;

ui::StuffBookUI::StuffBookUI()
{
    gtkTreeViewStuffBook = gtk_builder_get_object(GtkUserInterface::builder, "gtkTreeViewStuffBook");
    gtkScrolledWindowStuffbook = gtk_builder_get_object(GtkUserInterface::builder, "gtkScrolledWindowStuffbook");
    

    gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(gtkTreeViewStuffBook), true);

    g_signal_connect(gtkTreeViewStuffBook, "key-press-event", G_CALLBACK(static_cb_removeThing), this);

    createTreeView();
    updateTree();
    
    g_signal_connect(G_OBJECT(gtkTreeViewStuffBook), "row-activated", G_CALLBACK(static_cb_selectThing), this);
}


void ui::StuffBookUI::static_cb_selectThing(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data)
{
    reinterpret_cast<StuffBookUI*>(user_data)->cb_selectThing(tree_view, path, column, user_data);
}


gboolean ui::StuffBookUI::static_cb_removeThing(GtkWidget* widget, GdkEventKey* event, gpointer user_data)
{
  return reinterpret_cast<StuffBookUI*>(user_data)->cb_removeThing(widget, event, user_data);
}

void ui::StuffBookUI::createTreeView()
{
    GtkCellRenderer* renderer;
    GtkTreeViewColumn* col;

    col = gtk_tree_view_column_new();// create a new col //
    gtk_tree_view_column_set_sizing(col, GTK_TREE_VIEW_COLUMN_FIXED); // set col as fixed //
    
    gtk_tree_view_column_set_title(col, "Things"); // title, not used //
    gtk_tree_view_append_column(GTK_TREE_VIEW(gtkTreeViewStuffBook), col); // append col into tree view //

    renderer = gtk_cell_renderer_text_new(); // adjust how text is drawn using object properties //
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", 0);

    gtk_tree_view_set_show_expanders(GTK_TREE_VIEW(gtkTreeViewStuffBook), true); // show expanders //
    gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(gtkTreeViewStuffBook), true); // tree lines view //
        
}

void ui::StuffBookUI::updateTree()
{
    GtkTreeModel* model = fillTree(); // fil model //
    gtk_tree_view_set_model(GTK_TREE_VIEW(gtkTreeViewStuffBook), model); // set //
    g_object_unref(model);   // unref //
}

GtkTreeModel* ui::StuffBookUI::fillTree()
{
    GtkTreeStore* treestore; 
    GtkTreeIter toplevel;

    treestore = gtk_tree_store_new(1, G_TYPE_STRING); // type of store //
    auto mapType = gResources->getStuffBook(); // get stuffbook //

    for (auto it = mapType.begin(); it != mapType.end(); it++) // iterate through stuffbook //
    {
        gtk_tree_store_append(treestore, &toplevel, NULL); // top level //
        gtk_tree_store_set(treestore, &toplevel, 0, it->first.c_str(), -1); // set type //
        GtkTreeIter child;
        auto map = it->second; // access the sub dicionary [things] //
        for (auto it_j = map.begin(); it_j != map.end(); it_j++) // iterate through each thing into this type //
        {
            gtk_tree_store_append(treestore, &child, &toplevel); // add child //
            gtk_tree_store_set(treestore, &child, 0, it_j->first.c_str(), -1); // set name //
        }
    }

    return GTK_TREE_MODEL(treestore);
}

gboolean ui::StuffBookUI::cb_removeThing(GtkWidget* widget, GdkEventKey* event,gpointer user_data)
{
    // is delete key ? if no, do nothing //
    if (event->keyval != GDK_KEY_Delete)
    {
        gtk_statusbar_push(gAuxUI->getStatusBar(), 0, std::string("You can delete thing with key del!").c_str());
        return true;
    }    
     
    // commun variables //    
     GtkTreeSelection * gtkTreeSelection =  gtk_tree_view_get_selection(GTK_TREE_VIEW(widget)); // grab tree selection from mouse click //
     GtkTreeModel* model = nullptr;
     GtkTreeIter iter, parent;

     if(gtk_tree_selection_get_selected(gtkTreeSelection, &model, &iter)) // if true //
     {
         gchar* name,* type;
         
         gtk_tree_model_get(model, &iter, 0, &name, -1);
         // root parent //
         if (!gtk_tree_model_iter_parent(model, &parent, &iter)) // prevent delet a parent (thign type) //
         {
             gtk_statusbar_push(gAuxUI->getStatusBar(), 0, std::string("Select a thing to be deleted!").c_str());
             return true;
         }

         gtk_tree_model_get(model, &parent, 0, &type, -1);

         // find the chapter //
         auto it = gResources->getStuffBook().find(std::string(type));         
         if (it != gResources->getStuffBook().end())// founded //
         {
             // find the key //
             auto keyIt = (*it).second.find(std::string(name));
             if (keyIt != (*it).second.end() && !((*it).second.empty())) // founded //
             {
                // update model, erasing //
                (*it).second.erase(std::string(name));
                // update view
                updateTree();

                // delet all things from the mapUI //
                gMapUI->deletAllThingsFromTheMap(std::string(name));
                gMapUI->forceRedraw();

                // has elements ? //
                if ((*it).second.empty())
                {
                    gtk_statusbar_push(gAuxUI->getStatusBar(), 0, std::string("Thing " + std::string(name) + " [" + std::string(type) + "] deleted!").c_str());
                    return true;
                }

                // change gtkTreeView appearance //
                // try to find previous data //
                keyIt = (*it).second.lower_bound(std::string(name));
                // can be the first element //
                if (keyIt != (*it).second.end()) // founded //
                {
                    gAuxUI->searchThingByName((*keyIt).first);
                    std::string msg = "Thing ";
                    msg += name;
                    msg += " [";
                    msg += type;
                    msg += "] deleted!";

                    gtk_statusbar_push(gAuxUI->getStatusBar(), 0, std::string("Thing " + std::string(name) + " [" + std::string(type) + "] deleted!").c_str());
                    return true;
                }   
                // the erased is the last element, select the previous //
                else
                {
                    auto keyItr = (*it).second.rbegin();
                    gAuxUI->searchThingByName((*keyItr).first);
                    std::string msg = "Thing ";
                    msg += name;
                    msg += " [";
                    msg += type;
                    msg += "] deleted!";

                    gtk_statusbar_push(gAuxUI->getStatusBar(), 0, std::string("Thing " + std::string(name) + " [" + std::string(type) + "] deleted!").c_str());
                    return true;
                }
             }
         }
        
         g_free(name);
     }

    return true;
}


void ui::StuffBookUI::cb_selectThing(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data)
{
    GtkTreeSelection* gtkTreeSelection = gtk_tree_view_get_selection(tree_view); // grab tree selection from mouse click //
    GtkTreeModel* model = nullptr;
    GtkTreeIter iter, parent;
   
    // we found the item //
    if (gtk_tree_selection_get_selected(gtkTreeSelection, &model, &iter)) 
    {
        gchar* thingName, * thingType;        
        gtk_tree_model_get(model, &iter, 0, &thingName, -1); // get Thing name // 
        
        // grab parent position //
        if (gtk_tree_model_iter_parent(model, &parent, &iter) == FALSE) // we are tryning to select a parent //
        {
            gAuxUI->printMsg("You need to select an Thing Object!");
            return;
        }
        // grab parent data //
        gtk_tree_model_get(model, &parent, 0, &thingType, -1); // get Thing type // 
        
        auto itType = gResources->getStuffBook().find(std::string(thingType));
        if (itType != gResources->getStuffBook().end())// founded type //
        {
            auto map = itType->second;
            // let's found the Thing by name //
            auto mapThing = map.find(std::string(thingName));
            if (mapThing != map.end())// founded //
            {
                lastThingSelected = mapThing->second.getName();
                gAuxUI->printMsg("Thing " + mapThing->second.getName() + " selected!");
                data::Thing t = mapThing->second;
                t.setStuffBookRefName(lastThingSelected);
                gMapUI->setDrawThingObj(t);
                gDrawingToolUI->setDrawingMode(def::DrawingToolMode::DRAWING_BRUSH);
            }
        }        
    }
}

void ui::StuffBookUI::deleteAllThings(std::string imgName)
{
  auto map = gResources->getStuffBook(); // get stuffbook //
  std::vector<std::string> namesToDelete;
  
  for (auto it = map.begin(); it != map.end(); it++) // iterate through stuffbook //
  {    
    for (auto it_thing = it->second.begin(); it_thing != it->second.end(); it_thing++) // iterate through each thing into this type //
    {
      if (it_thing->second.getImgObjPtr()->getName() == imgName)
      {
        namesToDelete.push_back(it_thing->second.getName()); // grab Thing name //
      }
    }
  }

  // all things to delete from the stuffbook user interface //
  for (int i = 0; i < namesToDelete.size(); i++)
  {
    // delete all things from the mapUI //
    gMapUI->deletAllThingsFromTheMap(namesToDelete[i]); // clean all deleted things from the map //
    gResources->delThing(namesToDelete[i]);
  }

  gMapUI->forceRedraw();
}

bool ui::StuffBookUI::getThingByName(std::string name, data::Thing& thing)
{
    auto map = gResources->getStuffBook(); // get stuffbook //
    
    for (auto it = map.begin(); it != map.end(); it++) // iterate through stuffbook //
    {
        for (auto it_thing = it->second.begin(); it_thing != it->second.end(); it_thing++) // iterate through each thing into this type //
        {
            if (it_thing->first == name)
            {
                thing = it_thing->second;
                return true;
            }
        }
    }
    return false;
}