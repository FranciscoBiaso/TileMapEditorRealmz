#include "ImgPackUI.h"

GObject* ui::ImgPackUI::gtkTreeViewImgPack = nullptr;
GObject* ui::ImgPackUI::gtkScrolledWindowImgPack = nullptr;

namespace GtkUserInterface { extern GtkBuilder* builder; }

extern data::MapResources* gResources;
extern ui::AuxUI* gAuxUI;
extern ui::ThingCreatorTool* gThingCreatorTool;
extern ui::GraphicsTool* gGraphicsTool;

#ifdef TME_DEBUG
extern DebugTextureAtlas* debugTextureAtlas;
#endif // TME_DEBUG

ui::ImgPackUI::ImgPackUI()
{
    gtkTreeViewImgPack = gtk_builder_get_object(GtkUserInterface::builder, "gtkTreeViewImgPack");
    gtkScrolledWindowImgPack = gtk_builder_get_object(GtkUserInterface::builder, "gtkScrolledWindowImgPack");
    
    g_signal_connect(gtkTreeViewImgPack, "row-activated", G_CALLBACK(cb_selectImg), NULL);
    g_signal_connect(gtkTreeViewImgPack, "key-press-event", G_CALLBACK(cb_removeImgObj), NULL);

    createTreeView();
    updateTree();
}

void ui::ImgPackUI::createTreeView()
{
    GtkCellRenderer* renderer;
    GtkTreeViewColumn* col;

    col = gtk_tree_view_column_new();// create a new col //
    gtk_tree_view_column_set_title(col, "ImgObj"); // title, not used //
    gtk_tree_view_column_set_sizing(col, GTK_TREE_VIEW_COLUMN_FIXED); // set col as fixed //

    gtk_tree_view_append_column(GTK_TREE_VIEW(gtkTreeViewImgPack), col); // append col into tree view //

    renderer = gtk_cell_renderer_text_new(); // adjust how text is drawn using object properties //
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", 0);

    gtk_tree_view_set_show_expanders(GTK_TREE_VIEW(gtkTreeViewImgPack), true); // show expanders //
    gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(gtkTreeViewImgPack), true); // tree lines view //
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

    treestore = gtk_tree_store_new(1, G_TYPE_STRING); // type of store //
    auto mapType = gResources->getImgPack().getStrucutre(); // get stuffbook //

    for (auto it = mapType.begin(); it != mapType.end(); it++) // iterate through stuffbook //
    {
        gtk_tree_store_append(treestore, &toplevel, NULL); // top level //
        gtk_tree_store_set(treestore, &toplevel, 0, it->getName().c_str(), -1); // set type //
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
        gResources->getImgPack().delImgObj(std::string(name));
        // update view
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
        auto it = gResources->getImgPack().find(std::string(imgObjName));
        gThingCreatorTool->setThingImgObjPtr(&(*it));        
        gThingCreatorTool->updateImgPixelArea();
    }        
}