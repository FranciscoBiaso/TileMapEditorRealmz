#include "ImgPackUI.h"
#include "MapUI.h"

GObject* ui::ImgPackUI::gtkTreeViewImgPack = nullptr;
GObject* ui::ImgPackUI::gtkScrolledWindowImgPack = nullptr;

namespace GtkUserInterface { extern GtkBuilder* builder; }

extern data::MapResources* gResources;
extern ui::AuxUI* gAuxUI;
extern ui::ThingCreatorTool* gThingCreatorTool;
extern ui::GraphicsTool* gGraphicsTool;
extern ui::MapUI* gMapUI;
extern ui::StuffBookUI* gStuffBook;

#ifdef TME_DEBUG
extern DebugTextureAtlas* debugTextureAtlas;
#endif // TME_DEBUG

ui::ImgPackUI::ImgPackUI()
{
    gtkTreeViewImgPack = gtk_builder_get_object(GtkUserInterface::builder, "gtkTreeViewImgPack");
    gtkScrolledWindowImgPack = gtk_builder_get_object(GtkUserInterface::builder, "gtkScrolledWindowImgPack");
    
    gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(gtkTreeViewImgPack), true); 

    g_signal_connect(gtkTreeViewImgPack, "row-activated", G_CALLBACK(static_cb_selectImg), this);
    g_signal_connect(gtkTreeViewImgPack, "key-press-event", G_CALLBACK(cb_removeImgObj), NULL);
    

    createTreeView();
    updateTree();
}

void ui::ImgPackUI::static_cb_selectImg(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data) 
{
    reinterpret_cast<ImgPackUI*>(user_data)->cb_selectImg(tree_view, path, column, user_data);
}

void ui::ImgPackUI::static_cb_clicked(GtkCellRendererText* renderer, char* path, char* new_text, gpointer user_data) 
{
    reinterpret_cast<ImgPackUI*>(user_data)->cb_clicked(renderer, path, new_text, user_data);
}

void ui::ImgPackUI::createTreeView()
{
    GtkCellRenderer* renderer,* renderer2;
    GtkTreeViewColumn* col, *col2;

    col = gtk_tree_view_column_new();// create a new col //
    col2 = gtk_tree_view_column_new();// create a new col //
    gtk_tree_view_column_set_title(col, "ImgObj"); // title, not used //
    gtk_tree_view_column_set_sizing(col, GTK_TREE_VIEW_COLUMN_FIXED); // set col as fixed //
    gtk_tree_view_column_set_title(col2, "PixelBuf"); // title, not used //
    gtk_tree_view_column_set_sizing(col2, GTK_TREE_VIEW_COLUMN_FIXED); // set col as fixed //

    gtk_tree_view_append_column(GTK_TREE_VIEW(gtkTreeViewImgPack), col); // append col into tree view //
    gtk_tree_view_append_column(GTK_TREE_VIEW(gtkTreeViewImgPack), col2); // append col into tree view //

    renderer = gtk_cell_renderer_text_new(); // adjust how text is drawn using object properties //
    g_object_set(renderer, "editable", TRUE, NULL);
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer,"text", 0);
    g_signal_connect(renderer, "edited", G_CALLBACK(static_cb_clicked), this);
    
    renderer2 = gtk_cell_renderer_pixbuf_new();
    gtk_tree_view_column_pack_start(col2, renderer2, TRUE);
    gtk_tree_view_column_add_attribute(col2, renderer2,"pixbuf", 1);


    gtk_tree_view_set_show_expanders(GTK_TREE_VIEW(gtkTreeViewImgPack), true); // show expanders //
    gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(gtkTreeViewImgPack), true); // tree lines view //
}



void ui::ImgPackUI::cb_clicked(GtkCellRendererText* renderer, char* path, char* new_text, gpointer user_data)
{
    GtkTreeSelection* gtkTreeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtkTreeViewImgPack)); // grab tree selection from mouse click //
    GtkTreeModel* model = nullptr;
    GtkTreeIter iter;

    auto sb = gResources->getStuffBook();
    
    // find img ptr into imgpack and update //
    for (auto it = sb.begin(); it != sb.end(); it++)
    {
        bool canBreak = false;
        data:std::map<std::string,data::Thing> & sub_map = it->second;
        for (auto it_j = sub_map.begin(); it_j != sub_map.end(); it_j++)
        {
            data::Thing& thing = it_j->second;
            data::ImgObj* ptr = thing.getImgObjPtr();
            if (ptr != nullptr && ptr->getName() == imgNameOnActiveRow)
            {
                ptr->setName(new_text);

                canBreak = true;
                break;
            }
        }
        if (canBreak) break;
    }
    // find img ptr into json and update//
    gResources->getImgPack().saveImgPackAsJsonFile();

    updateTree();

}

void ui::ImgPackUI::updateTree()
{
    GtkTreeModel* model = fillTree(); // fil model //
    gtk_tree_view_set_model(GTK_TREE_VIEW(gtkTreeViewImgPack), model); // set //
    g_object_unref(model);   // unref //
}

GtkTreeModel* ui::ImgPackUI::fillTree()
{
    GtkTreeStore* treestore;
    GtkTreeIter toplevel;

    treestore = gtk_tree_store_new(2, G_TYPE_STRING, GDK_TYPE_PIXBUF); // type of store //
    auto list = gResources->getImgPack().getStrucutre(); // get img pack //

    for (auto it = list.begin(); it != list.end(); it++) // iterate through img pack //
    {
        math::Vec2<int> ref = it->getRef(0);
        GdkPixbuf* pixelBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, 32, 32);
        gdk_pixbuf_copy_area(gResources->getImgPack().getTextureAtlas()->getPixelbuf(), ref.getX()*REALMZ_GRID_SIZE, ref.getY() * REALMZ_GRID_SIZE, 32, 32, pixelBuf, 0, 0);

        gtk_tree_store_append(treestore, &toplevel, NULL); // top level //
        gtk_tree_store_set(treestore, &toplevel, 0, it->getName().c_str(), 1, pixelBuf, -1); // set type //
        g_object_unref(pixelBuf);

        //gResources->getImgPack().getTextureAtlas()->getPixelbuf()        
    }

    return GTK_TREE_MODEL(treestore);
}

gboolean ui::ImgPackUI::cb_removeImgObj(GtkWidget* widget, GdkEventKey* event, gpointer   user_data)
{    
    // is delete key ? if no, do nothing //
    if (event->keyval != GDK_KEY_Delete)
    {
        gtk_statusbar_push(gAuxUI->getStatusBar(), 0, std::string("Press delete key to remove a ImgObj!").c_str());
        return TRUE;
    }

    // commun variables //    
    GtkTreeSelection* gtkTreeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget)); // grab tree selection from mouse click //
    GtkTreeModel* model = nullptr;
    GtkTreeIter iter, parent;

    if (gtk_tree_selection_get_selected(gtkTreeSelection, &model, &iter)) // if true //
    {
        gchar* name;
        gtk_tree_model_get(model, &iter, 0, &name, -1);        
        
        // * IMPORTANT * WE DELET ALL THINGS WITH THIS IMG POINTER BEFORE DELETE THIS IMG //
        // delete all things with this image from the stuffbook //
        gStuffBook->deleteAllThings(std::string(name));

        // update view //
        // we update the stuffbook after images pack update //
        gStuffBook->updateTree();

        gResources->getImgPack().delImgObj(std::string(name));
        // update view //
        updateTree();        

#ifdef TME_DEBUG
        debugTextureAtlas->surface = gdk_cairo_surface_create_from_pixbuf(gResources->getImgPack().getTextureAtlas()->getPixelbuf(), 0, NULL);
        gtk_widget_queue_draw(GTK_WIDGET(debugTextureAtlas->drawingArea));
#endif // TME_DEBUG
    }


    return TRUE;
}

void ui::ImgPackUI::cb_selectImg(GtkTreeView* tree_view,GtkTreePath* path,GtkTreeViewColumn* column, gpointer user_data)
{    

    GtkTreeSelection* gtkTreeSelection = gtk_tree_view_get_selection(tree_view); // grab tree selection from mouse click //
    GtkTreeModel* model = nullptr;
    GtkTreeIter iter, parent;

    if (gtk_tree_selection_get_selected(gtkTreeSelection, &model, &iter)) // if true //
    {
        gchar* imgObjName;
        gtk_tree_model_get(model, &iter, 0, &imgObjName, -1); // get ImgObj name //
        imgNameOnActiveRow = std::string(imgObjName);
        auto it = gResources->getImgPack().find(std::string(imgObjName));
        if (it != gResources->getImgPack().getImgVec().end())
        {
            gThingCreatorTool->updateImgPixelArea(&(*it));
        }
    }        
}