#ifdef TME_DEBUG
#include "DebugTextureAtlas.h"
#include "MapResources.h"
#include "GraphicsTool.h"
#include "ImgPackUI.h"
#include <cstdlib> // rand

GtkWidget* DebugTextureAtlas::drawingArea;
cairo_surface_t* DebugTextureAtlas::surface = nullptr;
GObject* DebugTextureAtlas::p_window = nullptr;

int DebugTextureAtlas::imgNamesInt = 0;
std::vector<std::string> DebugTextureAtlas::imgNames;
std::stack<std::string> DebugTextureAtlas::operations;

namespace GtkUserInterface { extern GtkBuilder* builder; }

extern data::MapResources* gResources;
extern ui::GraphicsTool* gGraphicsTool;
extern ui::ImgPackUI* gImgPackUI;

DebugTextureAtlas::DebugTextureAtlas(GdkPixbuf * pixlBuff)
{
    p_window = G_OBJECT(gtk_builder_get_object(GtkUserInterface::builder, "DebugWindow"));
    gtk_window_set_title(GTK_WINDOW(p_window), "Debug window");
    gtk_window_set_default_size(GTK_WINDOW(p_window), 300, 300);
    //gtk_window_set_position(GTK_WINDOW(p_window), GTK_WIN_POS_CENTER);
    
    drawingArea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(p_window), drawingArea);
    g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(cb_draw_callback), NULL);
    surface = gdk_cairo_surface_create_from_pixbuf(pixlBuff, 0, NULL);

    const gchar* filename = "C:\\Users\\Francisco\\source\\repos\\TME\\Tile-Map-Editor---Realmz\\Release\\imgs\\texture-atlas.png";
    gGraphicsTool->setPixBufSrc(gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file(filename))));

    gtk_widget_show_all(GTK_WIDGET(p_window));
    //g_timeout_add(75, push_operations,NULL);
    //g_timeout_add(75, pop_operations, NULL);
}

gboolean DebugTextureAtlas::push_operations(gpointer data)
{
    int countImgs = gResources->getImgPack().getCountImgs(); // count imgs //
    int random = std::rand() % 100; // random //
    int maxImgs = 40 + std::rand() % 11; // random [40,50] //
    
    // + add operations near 0 imgs // 
    // + del operations near maxImgs //
    // 0 <-- del add --> maxImgs //
    if(random >= countImgs/ maxImgs * 80)
        operations.push("add");
    else
        operations.push("del");
    return TRUE;
}

gboolean DebugTextureAtlas::pop_operations(gpointer data)
{
    // check if container is not empty //
    if (!operations.empty()) 
    {
        std::string op = operations.top(); //get top value //
        operations.pop(); // remove top //
        if (op == "add") // add //
            addImgs();
        else if (op == "del") // remove //
            remImgs();
    }
    return TRUE;
}

gboolean DebugTextureAtlas::cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    // draw surface //
    if (surface != nullptr)
    {
        gdk_cairo_set_source_pixbuf(cr,gResources->getImgPack().getTextureAtlas()->getPixelbuf(), 0, 0);
        cairo_paint(cr);
    }
    return TRUE;
}

gboolean DebugTextureAtlas::addImgs()
{
    // simulate ims that will be added into this texture atlas //
    gGraphicsTool->simulate();
    
    imgNames.push_back(std::to_string(imgNamesInt)); // add into vector //
    imgNamesInt++; // ++ //

    // add into ImgPack //
    gResources->getImgPack().addImgObj(imgNames[imgNames.size()-1], gGraphicsTool->getPixelBufDest(), gGraphicsTool->getImgFormat());
    
    // force draw //
    gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
    return TRUE;
}

gboolean DebugTextureAtlas::remImgs()
{
    // if container is not empty //
    if (!imgNames.empty())
    {   
        int random = std::rand() % imgNames.size(); // random [0,img size[ //
        
        imgNames.erase(imgNames.begin() + random); // erase from this vector //
        
        gResources->getImgPack().delImgObj(imgNames[random]); // erase from ImgPack //
        //gImgPackUI->updateTree(); // update tree view //

        // force draw //
        gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
        
    }
    return TRUE;
}
#endif