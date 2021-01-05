#include "GraphicsTool.h"
#include <cstdlib>

GObject* ui::GraphicsTool::gtkFileChooserButtonImg = nullptr;
GObject* ui::GraphicsTool::gtkImgInput = nullptr;
GObject* ui::GraphicsTool::gtkFrameImg = nullptr;
GObject* ui::GraphicsTool::gtkFrameImgView = nullptr;
GObject* ui::GraphicsTool::gtkEntryImgName = nullptr;
GObject* ui::GraphicsTool::gtkButtonAddImgName = nullptr;
GObject* ui::GraphicsTool::gtkTreeViewImgObj = nullptr;
GObject* ui::GraphicsTool::gtkButtonCreateImg = nullptr;
GObject* ui::GraphicsTool::gtkButtonSplitImg = nullptr;
GObject* ui::GraphicsTool::gtkScrolledWindowTileset = nullptr;
GObject* ui::GraphicsTool::gtkCheckButtonGrid = nullptr;
GObject* ui::GraphicsTool::gtkToggleButton32x32 = nullptr;
GObject* ui::GraphicsTool::gtkToggleButton32x64 = nullptr;
GObject* ui::GraphicsTool::gtkToggleButton64x32 = nullptr;
GObject* ui::GraphicsTool::gtkToggleButton64x64 = nullptr;
GObject* ui::GraphicsTool::gtkViewportImgSrc = nullptr;
GObject* ui::GraphicsTool::gtkFrameGraphicsTool = nullptr;

GtkWidget* ui::GraphicsTool::drawingAreaImgSrc = nullptr;
GtkWidget* ui::GraphicsTool::drawingAreaImgDst = nullptr;
GdkPixbuf* ui::GraphicsTool::pixelBufImgSrc = nullptr;
GdkPixbuf* ui::GraphicsTool::pixelBufImgDest = nullptr;
GdkPixbuf* ui::GraphicsTool::pixelBufImgDest_background = nullptr;
GdkPixbuf* ui::GraphicsTool::dragIcon32x32 = nullptr;

int ui::GraphicsTool::xPosHighlightSquare = 0;
int ui::GraphicsTool::yPosHighlightSquare = 0;
int ui::GraphicsTool::xPosDropSquare = 0;
int ui::GraphicsTool::yPosDropSquare = 0;
int ui::GraphicsTool::MAX_IMG_WIDGET_WIDTH = 320;
int ui::GraphicsTool::MAX_IMG_WIDGET_HEIGHT = 320;
int ui::GraphicsTool::imgCursor = 0;
bool ui::GraphicsTool::canDrawSelectedSquare = false;

std::string ui::GraphicsTool::imgName = "empty";
def::IMG_SIZE ui::GraphicsTool::imgFormat = def::IMG_SIZE::IMG_SIZE_32X32;

cairo_surface_t*  ui::GraphicsTool::surfaceScr = nullptr;
cairo_surface_t* ui::GraphicsTool::surfaceDst = nullptr;
cairo_surface_t* ui::GraphicsTool::surfaceDst_background = nullptr;

GtkTargetEntry ui::GraphicsTool::dragTarget = { (gchar*)"targetImg",0, 0 };

namespace GtkUserInterface { extern GtkBuilder* builder;}
extern data::MapResources* gResources;
#ifdef TME_DEBUG
extern DebugTextureAtlas* debugTextureAtlas;
#endif
extern ui::ImgPackUI* gImgPackUI;
extern ui::AuxUI* gAuxUI;

ui::GraphicsTool::GraphicsTool()
{
    // initialize timer //
    g_timeout_add(600, timerChangeSquareData, NULL);
    gtkFrameGraphicsTool = gtk_builder_get_object(GtkUserInterface::builder, "frame1");
    gtkFileChooserButtonImg = gtk_builder_get_object(GtkUserInterface::builder, "gtkFileChooserButtonImg");
    gtkImgInput = gtk_builder_get_object(GtkUserInterface::builder, "gtkImgInput");
    gtkFrameImg = gtk_builder_get_object(GtkUserInterface::builder, "gtkFrameImg");
    gtkFrameImgView = gtk_builder_get_object(GtkUserInterface::builder, "gtkFrameImgView");
    gtkViewportImgSrc= gtk_builder_get_object(GtkUserInterface::builder, "gtkViewportImgSrc");
    gtkTreeViewImgObj = gtk_builder_get_object(GtkUserInterface::builder, "gtkTreeViewImgObj");
    gtkButtonCreateImg = gtk_builder_get_object(GtkUserInterface::builder, "gtkButtonCreateImg");
    gtkButtonSplitImg = gtk_builder_get_object(GtkUserInterface::builder, "gtkButtonSplitImg");
    gtkEntryImgName = gtk_builder_get_object(GtkUserInterface::builder, "gtkEntryImgName");

    /* toggle buttons */
    gtkCheckButtonGrid = gtk_builder_get_object(GtkUserInterface::builder, "gtkCheckButtonGrid");
    gtkToggleButton32x32 = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButton32x32");
    gtkToggleButton32x64 = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButton32x64");
    gtkToggleButton64x32 = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButton64x32");
    gtkToggleButton64x64 = gtk_builder_get_object(GtkUserInterface::builder, "gtkToggleButton64x64");
    /* preset toggle buttons state */
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32), TRUE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32), FALSE);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64), FALSE);
    /* connect toggle buttons state */
    g_signal_connect(gtkCheckButtonGrid, "toggled", G_CALLBACK(cb_signalGtkToggleButtonGrid), gtkCheckButtonGrid);
    g_signal_connect(gtkToggleButton32x32, "toggled", G_CALLBACK(cb_signalGtkToggleButton32x32), gtkToggleButton32x32);
    g_signal_connect(gtkToggleButton32x64, "toggled", G_CALLBACK(cb_signalGtkToggleButton32x64), gtkToggleButton32x64);
    g_signal_connect(gtkToggleButton64x32, "toggled", G_CALLBACK(cb_signalGtkToggleButton64x32), gtkToggleButton64x32);
    g_signal_connect(gtkToggleButton64x64, "toggled", G_CALLBACK(cb_signalGtkToggleButton64x64), gtkToggleButton64x64);

    pixelBufImgDest = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, REALMZ_GRID_SIZE * 2, REALMZ_GRID_SIZE * 2);
    gdk_pixbuf_fill(pixelBufImgDest, 0x00000000); // clean buffer //   

    GError* err = NULL;
    pixelBufImgDest_background = gdk_pixbuf_new_from_file("ui_imgs//surface32x32.png", &err);
    
    dragIcon32x32 = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE);

    setDrawingAreaImgDst(GTK_WIDGET(gtkFrameImgView));

    std::string path = g_win32_get_package_installation_directory_of_module(NULL);
    path += "\\imgs\\";
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(gtkFileChooserButtonImg), path.c_str());
    gtk_file_chooser_set_action(GTK_FILE_CHOOSER(gtkFileChooserButtonImg), GTK_FILE_CHOOSER_ACTION_OPEN);
    //gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(gtkFileChooserButtonImg), TRUE);

    /* bind check button to grid action (show or hide) */
    g_signal_connect(gtkButtonCreateImg, "clicked", G_CALLBACK(cb_createImgObj), NULL);
    g_signal_connect(gtkButtonSplitImg, "clicked", G_CALLBACK(cb_splitImgObj), NULL);

    /* bind cb button create img obj */
    g_signal_connect(gtkFileChooserButtonImg, "file-set", G_CALLBACK(cb_onFileSet), gtkViewportImgSrc);
    g_signal_connect(gtkEntryImgName, "changed", G_CALLBACK(cb_updateImgObjName), NULL);


    createTreeViewImgObj();
    updateTreeImgObj();

    // update the surface //
    setDstSurfaceBGFromDstPixelbufBG();
}

gboolean ui::GraphicsTool::cb_MotionNotify(GtkWidget* widget, GdkEventMotion* e, gpointer   user_data)
{
    xPosHighlightSquare = (int)(e->x/REALMZ_GRID_SIZE) * REALMZ_GRID_SIZE;
    yPosHighlightSquare = (int)(e->y/REALMZ_GRID_SIZE) * REALMZ_GRID_SIZE;
    if (drawingAreaImgSrc != nullptr)
        gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgSrc));
    return TRUE;    
}

void ui::GraphicsTool::simulate()
{
    xPosHighlightSquare = (rand() % 15) * REALMZ_GRID_SIZE;
    yPosHighlightSquare = (rand() % 15) * REALMZ_GRID_SIZE;
    int r = rand() % 2;
    if (r == 0)
    {
        imgFormat = def::IMG_SIZE::IMG_SIZE_32X32;
        gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2);

    }
    if (r == 1)
    {
        imgFormat = def::IMG_SIZE::IMG_SIZE_64X64;
        gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, 0, 0);
        gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, REALMZ_GRID_SIZE, 0);
        gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, 0, REALMZ_GRID_SIZE);
        gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE);
          
    }
}

gboolean ui::GraphicsTool::cb_clickNotify(GtkWidget* widget, GdkEventButton* event, gpointer   user_data)
{
    // double click //
    if (event->type == GDK_DOUBLE_BUTTON_PRESS)
    {      
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32)) == TRUE)
        {
            gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2);
            setDstSurfaceFromDstPixelbuf();
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE)
        {
            gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, REALMZ_GRID_SIZE / 2,imgCursor * REALMZ_GRID_SIZE);
            setDstSurfaceFromDstPixelbuf();
            rightShiftCursor();
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
        {
            gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, imgCursor * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE / 2);
            setDstSurfaceFromDstPixelbuf();
            rightShiftCursor();
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
        {
            if(imgCursor <= 1)
                gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, imgCursor * REALMZ_GRID_SIZE, 0);
            else
                gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, (imgCursor-2) * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE);
            setDstSurfaceFromDstPixelbuf();
            rightShiftCursor();
        }

        gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));
    }

    return TRUE;
}

void ui::GraphicsTool::cb_toggleButtonChangeGrid(GtkToggleButton* togglebutton, gpointer data)
{
    if (drawingAreaImgSrc != nullptr)
        gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgSrc));
}

gboolean ui::GraphicsTool::cb_draw_callback_img_src(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    // highlight Square color //
    GdkRGBA HLSColor; HLSColor.red = 0; HLSColor.green = 0; HLSColor.blue = 0.85; HLSColor.alpha = 0.25;
    // backgrond color //
    GdkRGBA bgColor; bgColor.red = 0.5; bgColor.green = 0.5; bgColor.blue = 0.5; bgColor.alpha = 0.5;

    // draw background //
     cairo_set_source_rgba(cr, bgColor.red, bgColor.green,bgColor.blue,bgColor.alpha);
     cairo_paint(cr);

    guint width = gtk_widget_get_allocated_width(widget);
    guint height = gtk_widget_get_allocated_height(widget);
    
    //int w = cairo_image_surface_get_width(surface);
    //int h = cairo_image_surface_get_height(surface);

    if (surfaceScr != nullptr)
    {
        // draw surface into widget region //
        cairo_set_source_surface(cr, surfaceScr, 0, 0);
        cairo_paint(cr);
    }

    // Draw the highlighted square //
    drawSquare(cr, xPosHighlightSquare, yPosHighlightSquare,REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);

    // draw grid //
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkCheckButtonGrid)) == TRUE)
        drawGrid(cr, width, height, REALMZ_GRID_SIZE);

    return FALSE;
}

gboolean ui::GraphicsTool::cb_draw_callback_img_dst(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    // highlight Square color //
    GdkRGBA HLSColor; HLSColor.red = 0; HLSColor.green = 0; HLSColor.blue = 0.85; HLSColor.alpha = 0.15;
    // backgrond color //
    GdkRGBA bgColor; bgColor.red = 0.5; bgColor.green = 0.5; bgColor.blue = 0.5; bgColor.alpha = 0.5;
    // square color //
    GdkRGBA sColor; sColor.red = 0.35; sColor.green = 0.35; sColor.blue = 0.35; sColor.alpha = 0.12;

    // draw background //
    
    /*
    cairo_pattern_t* pattern;
    pattern = cairo_pattern_create_linear(0.0, 0.0, 2 * REALMZ_GRID_SIZE, 2 * REALMZ_GRID_SIZE);
    cairo_pattern_add_color_stop_rgba(pattern, 0, 1, 0, 0, 0.15);
    cairo_pattern_add_color_stop_rgba(pattern, 1, 1, 1, 0, 0.00);
    cairo_set_source_rgba(cr, bgColor.red, bgColor.green, bgColor.blue, bgColor.alpha);
    cairo_set_source(cr, pattern);
    cairo_paint(cr);
    */

    if (surfaceDst_background != nullptr)
    {
        cairo_set_source_surface(cr, surfaceDst_background, 0, 0);
        cairo_paint(cr);
    }
    // draw background square (32x32,32x64,64x32 or 64x64)
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32)) == TRUE)       drawImg32x32(cr, sColor);
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE)  drawImg32x64(cr, sColor);
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)  drawImg64x32(cr, sColor);
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)  drawImg64x64(cr, sColor);
    
    if (surfaceDst != nullptr)
    {
        cairo_set_source_surface(cr, surfaceDst, 0, 0);
        cairo_paint(cr);
    }

    // highligh square //
    if (canDrawSelectedSquare)
    {
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32)) == TRUE)
        {
            drawSquare(cr, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE)
        {
            drawSquare(cr, REALMZ_GRID_SIZE / 2, imgCursor * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
        {
            drawSquare(cr, imgCursor * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
        {
            if (imgCursor <= 1)
                drawSquare(cr, imgCursor * REALMZ_GRID_SIZE, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);
            else
                drawSquare(cr, (imgCursor - 2) * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, HLSColor);
        }
    }
    return FALSE;
}

def::ReturnMsg ui::GraphicsTool::loadImgFromFile()
{
    gchar* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gtkFileChooserButtonImg));
    
    if (filename == NULL)
        return def::ReturnMsg::FILE_NOT_SELECTED;

    pixelBufImgSrc = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file(filename)));
    if (pixelBufImgSrc == NULL)
        return def::ReturnMsg::IMAGE_EMPTY;

    return def::ReturnMsg::FILE_OK;
}

void ui::GraphicsTool::setSrcSurfaceFromScrPixelbuf()
{
    surfaceScr = gdk_cairo_surface_create_from_pixbuf(pixelBufImgSrc, 0, NULL);
}

void ui::GraphicsTool::setDstSurfaceFromDstPixelbuf()
{
    surfaceDst = gdk_cairo_surface_create_from_pixbuf(pixelBufImgDest, 0, NULL);
}

void ui::GraphicsTool::setDstSurfaceBGFromDstPixelbufBG()
{
    surfaceDst_background = gdk_cairo_surface_create_from_pixbuf(pixelBufImgDest_background, 0, NULL);
}

void ui::GraphicsTool::setDrawingAreaImgDst(GtkWidget* widget)
{
    if (drawingAreaImgDst == nullptr)
    {
        drawingAreaImgDst = gtk_drawing_area_new();
        gtk_widget_add_events(drawingAreaImgDst, GDK_ALL_EVENTS_MASK);
        
        gtk_drag_dest_set(drawingAreaImgDst, GTK_DEST_DEFAULT_MOTION, &dragTarget, 1, GDK_ACTION_COPY);

        gtk_container_add(GTK_CONTAINER(widget), drawingAreaImgDst);


        g_signal_connect(G_OBJECT(drawingAreaImgDst), "draw", G_CALLBACK(cb_draw_callback_img_dst), NULL);
        g_signal_connect(G_OBJECT(drawingAreaImgDst), "drag-drop", G_CALLBACK(cb_dragDrop), NULL);                
    }
}

void ui::GraphicsTool::setDrawingAreaImgScr(GtkWidget * widget)
{
    drawingAreaImgSrc = gtk_drawing_area_new();
    gtk_widget_add_events(drawingAreaImgSrc, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(drawingAreaImgSrc, GDK_BUTTON_PRESS_MASK);
    gtk_drag_source_set(drawingAreaImgSrc, GDK_BUTTON1_MASK, &dragTarget, 1, GDK_ACTION_COPY);
    gtk_drag_source_set_icon_pixbuf(drawingAreaImgSrc, dragIcon32x32);

    gtk_container_add(GTK_CONTAINER(widget), drawingAreaImgSrc);

    g_signal_connect(G_OBJECT(drawingAreaImgSrc), "draw", G_CALLBACK(cb_draw_callback_img_src), NULL);
    g_signal_connect(G_OBJECT(drawingAreaImgSrc), "motion-notify-event", G_CALLBACK(cb_MotionNotify), NULL);
    g_signal_connect(G_OBJECT(drawingAreaImgSrc), "button-press-event", G_CALLBACK(cb_clickNotify), NULL);
    g_signal_connect(G_OBJECT(drawingAreaImgSrc), "drag-begin", G_CALLBACK(cb_dragBegin), NULL);

    gtk_widget_show_all(GTK_WIDGET(gtk_widget_get_parent(drawingAreaImgSrc)));
}

void ui::GraphicsTool::cb_onFileSet(GtkFileChooserButton* widget, gpointer data)
{
    if (loadImgFromFile() != def::ReturnMsg::FILE_OK)
    {
        std::cout << "Error to load img from file!"<<std::endl;// used until create an txt msg manipulator //
        return;
    }

    setSrcSurfaceFromScrPixelbuf();
    if (drawingAreaImgSrc == nullptr)
    {
        setDrawingAreaImgScr(GTK_WIDGET(data));
    }
    int width = gdk_pixbuf_get_width(pixelBufImgSrc);
    int height = gdk_pixbuf_get_height(pixelBufImgSrc);
    gtk_widget_set_size_request(GTK_WIDGET(drawingAreaImgSrc), width, height);
    gtk_widget_set_size_request(GTK_WIDGET(gtkFrameImg), std::min(width,MAX_IMG_WIDGET_WIDTH), std::min(height,MAX_IMG_WIDGET_HEIGHT));
    gtk_widget_show_all(GTK_WIDGET(gtk_widget_get_parent(drawingAreaImgSrc)));
}

void ui::GraphicsTool::cb_signalGtkToggleButtonGrid(GtkToggleButton* togglebutton, gpointer user_data)
{
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgSrc));
}

void ui::GraphicsTool::cb_signalGtkToggleButton32x32(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64), FALSE);
       
    }
    imgFormat = def::IMG_SIZE::IMG_SIZE_32X32;
    updateTreeImgObj(); // update tree view //
    gdk_pixbuf_fill(pixelBufImgDest, 0x00000000); // clean buffer //
    setDstSurfaceFromDstPixelbuf();

    GError* err = NULL;
    g_object_unref(pixelBufImgDest_background);
    pixelBufImgDest_background = gdk_pixbuf_new_from_file("ui_imgs//surface32x32.png", &err);

    setDstSurfaceBGFromDstPixelbufBG();
    resetCursor();
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));
}

void ui::GraphicsTool::cb_signalGtkToggleButton32x64(GtkToggleButton* togglebutton, gpointer user_data)
{
    return;
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64), FALSE);        
    }
    imgFormat = def::IMG_SIZE::IMG_SIZE_32X64;
    updateTreeImgObj(); // update tree view //
    gdk_pixbuf_fill(pixelBufImgDest, 0x00000000); // clean buffer //
    setDstSurfaceFromDstPixelbuf();

    GError* err = NULL;
    g_object_unref(pixelBufImgDest_background);
    pixelBufImgDest_background = gdk_pixbuf_new_from_file("ui_imgs//surface32x64.png", &err);
    setDstSurfaceBGFromDstPixelbufBG();

    resetCursor();
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));
}

void ui::GraphicsTool::cb_signalGtkToggleButton64x32(GtkToggleButton* togglebutton, gpointer user_data)
{
    return;
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64), FALSE);
    }
    imgFormat = def::IMG_SIZE::IMG_SIZE_64X32;
    updateTreeImgObj(); // update tree view //
    gdk_pixbuf_fill(pixelBufImgDest, 0x00000000); // clean buffer //
    setDstSurfaceFromDstPixelbuf();
    GError* err = NULL;
    g_object_unref(pixelBufImgDest_background);
    pixelBufImgDest_background = gdk_pixbuf_new_from_file("ui_imgs//surface64x32.png", &err);
    setDstSurfaceBGFromDstPixelbufBG();
    resetCursor();
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));
}

void ui::GraphicsTool::cb_signalGtkToggleButton64x64(GtkToggleButton* togglebutton, gpointer user_data)
{
    return;
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32), FALSE);
    }
    imgFormat = def::IMG_SIZE::IMG_SIZE_64X64;
    updateTreeImgObj(); // update tree view //
    gdk_pixbuf_fill(pixelBufImgDest, 0x00000000); // clean buffer //
    setDstSurfaceFromDstPixelbuf();    
    GError* err = NULL;
    g_object_unref(pixelBufImgDest_background);
    pixelBufImgDest_background = gdk_pixbuf_new_from_file("ui_imgs//surface64x64.png", &err);
    setDstSurfaceBGFromDstPixelbufBG();
    resetCursor();
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));
}

void ui::GraphicsTool::drawGrid(cairo_t* cr, int w, int h, int gridSize)
{
    cairo_set_line_width(cr, 2);
    //double dashs[2] = { 1.0, 3.0 };
    //cairo_set_dash(cr, dashs, 2, 0);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
    cairo_set_source_rgba(cr, 0.95,0.95,0.1,0.95);

    int countRows = h / gridSize;
    int countCols = w / gridSize;
    for (int l = 0; l <= countRows; l++)
    {
        // draw vertical line //
        cairo_move_to(cr, 0, l * gridSize);
        cairo_line_to(cr, w, l * gridSize);

        for (int c = 0; c <= countCols; c++)
        {
            cairo_move_to(cr, c * gridSize, 0);
            cairo_line_to(cr, c * gridSize, h);
        }

    }
    cairo_stroke(cr);
    cairo_fill(cr);
}

void ui::GraphicsTool::drawSquare(cairo_t* cr, int x, int y, int w, int h, GdkRGBA color)
{
    cairo_rectangle(cr, x, y, w, h);
    cairo_set_source_rgba(cr, color.red, color.green, color.blue , color.alpha);
    cairo_fill(cr);
}

void ui::GraphicsTool::drawImg32x32(cairo_t* cr, GdkRGBA color)
{
    drawSquare(cr, REALMZ_GRID_SIZE/2, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
}

void ui::GraphicsTool::drawImg32x64(cairo_t* cr, GdkRGBA color)
{
    drawSquare(cr, REALMZ_GRID_SIZE / 2,0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
    drawSquare(cr, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
}

void ui::GraphicsTool::drawImg64x32(cairo_t* cr, GdkRGBA color)
{
    drawSquare(cr, 0, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
    drawSquare(cr, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
}

void ui::GraphicsTool::drawImg64x64(cairo_t* cr, GdkRGBA color)
{
    drawSquare(cr, 0, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
    drawSquare(cr, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
    drawSquare(cr, REALMZ_GRID_SIZE, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
    drawSquare(cr, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, color);
}

void ui::GraphicsTool::rightShiftCursor()
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE || gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
        imgCursor = (imgCursor+1) % 2;  // [0,1]
    else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
        imgCursor = (imgCursor + 1) % 4; // [0,1,2,3]
}

void ui::GraphicsTool::leftShiftCursor()
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE || gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
    {
        if (imgCursor == 0)
            imgCursor = 1;
        else
            imgCursor = (imgCursor - 1) % 2;  // [0,1]
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
    {
        if (imgCursor == 0)
            imgCursor = 3;
        else
            imgCursor = (imgCursor - 1) % 2;  // [0,1]
    }       
}

void ui::GraphicsTool::resetCursor()
{
    imgCursor = 0;
}

void ui::GraphicsTool::setCursor(int x, int y)
{
    int xTmp = (int)(x / REALMZ_GRID_SIZE) * REALMZ_GRID_SIZE;
    int yTmp = (int)(y / REALMZ_GRID_SIZE) * REALMZ_GRID_SIZE;


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32)) == TRUE)
    {
        imgCursor = 0;
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE)
    {
        if (yTmp == 0)
            imgCursor = 0;
        else if(yTmp == 32)
            imgCursor = 1;
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
    {
        if (xTmp == 0)
            imgCursor = 0;
        else if (xTmp == 32)
            imgCursor = 1;
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
    {
        if (xTmp == 0 && yTmp == 0)
            imgCursor = 0;
        else if (xTmp == 32 && yTmp == 0)
            imgCursor = 1;
        else if (xTmp == 0 && yTmp == 32)
            imgCursor = 2;
        else if (xTmp == 32 && yTmp == 32)
            imgCursor = 3;
    }
}

gboolean ui::GraphicsTool::timerChangeSquareData(gpointer data)
{
    if (canDrawSelectedSquare)
        canDrawSelectedSquare = false;
    else
        canDrawSelectedSquare = true;
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));

    return TRUE;
}

void ui::GraphicsTool::cb_dragBegin(GtkWidget* widget, GdkDragContext* context, gpointer user_data)
{
    gdk_pixbuf_copy_area(pixelBufImgSrc, xPosHighlightSquare, yPosHighlightSquare, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, dragIcon32x32, 0, 0);
}

gboolean  ui::GraphicsTool::cb_dragDrop(GtkWidget* widget, GdkDragContext* context,
        gint            x,
        gint            y,
        guint           time,
        gpointer        user_data)
{
    xPosDropSquare = (int)(x / REALMZ_GRID_SIZE) * REALMZ_GRID_SIZE;
    yPosDropSquare = (int)(y / REALMZ_GRID_SIZE) * REALMZ_GRID_SIZE;
    setCursor(xPosDropSquare, yPosDropSquare);
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32)) == TRUE)
    {
        gdk_pixbuf_copy_area(dragIcon32x32, 0, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2);        
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE)
    {
        gdk_pixbuf_copy_area(dragIcon32x32, 0, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, REALMZ_GRID_SIZE / 2, yPosDropSquare);  
        rightShiftCursor();
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
    {
        gdk_pixbuf_copy_area(dragIcon32x32, 0, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, xPosDropSquare, REALMZ_GRID_SIZE / 2);
        rightShiftCursor();
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
    {
        gdk_pixbuf_copy_area(dragIcon32x32, 0, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, xPosDropSquare, yPosDropSquare);
        rightShiftCursor();
    }

    setDstSurfaceFromDstPixelbuf();
    setDstSurfaceBGFromDstPixelbufBG();
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgDst));
    return TRUE;
}

void ui::GraphicsTool::updateTreeImgObj()
{
    GtkTreeModel* model = fillTreeImgObj();
    gtk_tree_view_set_model(GTK_TREE_VIEW(gtkTreeViewImgObj), model);
    g_object_unref(model);
}

GtkTreeModel* ui::GraphicsTool::fillTreeImgObj()
{
    // fill tree view thing obj //
    GtkTreeStore* treestore;
    GtkTreeIter toplevel;

    treestore = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel, 0, "name:", 1, imgName.c_str(), -1);

    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel, 0, "size:", 1, getSizeAsString().c_str(), -1);


    return GTK_TREE_MODEL(treestore);
}

void ui::GraphicsTool::createTreeViewImgObj()
{
    GtkCellRenderer* renderer, *renderer2;
    GtkTreeViewColumn* col, *col2;

    col = gtk_tree_view_column_new(); // create col //
    gtk_tree_view_append_column(GTK_TREE_VIEW(gtkTreeViewImgObj), col); // append //

    renderer = gtk_cell_renderer_text_new(); // adjust how text is drawn using object properties //
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer, "text", 0);

    col2 = gtk_tree_view_column_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(gtkTreeViewImgObj), col2);

    renderer2 = gtk_cell_renderer_text_new(); // adjust how text is drawn using object properties //
    gtk_tree_view_column_pack_start(col2, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col2, renderer, "text", 1);

    gtk_tree_view_set_show_expanders(GTK_TREE_VIEW(gtkTreeViewImgObj), true); // expanders //
    gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(gtkTreeViewImgObj), true); // tree lines //
}



void ui::GraphicsTool::cb_splitImgObj(GtkWidget* widget, gpointer data)
{

    int width = gdk_pixbuf_get_width(pixelBufImgSrc);
    int height = gdk_pixbuf_get_height(pixelBufImgSrc);

    gchar* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gtkFileChooserButtonImg));
    std::string strFileName = filename;
    std::string prefix = strFileName.substr(strFileName.find_last_of("\\") + 1);
    prefix = prefix.substr(0,prefix.find_last_of("."));
  
    int imgName = 1;
    for (int h = 0; h < height; h += 32)
    {
        for (int c = 0; c < width; c += 32)
        {
            gdk_pixbuf_copy_area(pixelBufImgSrc, c, h, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2);
            gResources->getImgPack().addImgObj(prefix + " " + std::to_string(imgName), pixelBufImgDest, def::IMG_SIZE::IMG_SIZE_32X32);
            imgName++;
        }
    }    

    gImgPackUI->updateTree(); // update tree view //
}

void ui::GraphicsTool::cb_createImgObj(GtkWidget* widget, gpointer data)
{
    auto it = gResources->getImgPack().find(std::string(imgName));
    
    if (it == gResources->getImgPack().getImgVec().end())
    {
        gResources->getImgPack().addImgObj(imgName, pixelBufImgDest, imgFormat);
        gImgPackUI->updateTree(); // update tree view //
    }
   
#ifdef TME_DEBUG
    debugTextureAtlas->surface = gdk_cairo_surface_create_from_pixbuf(gResources->getImgPack().getTextureAtlas()->getPixelbuf(), 0, NULL);
    gtk_widget_queue_draw(GTK_WIDGET(debugTextureAtlas->drawingArea));
#endif // TME_DEBUG
}

void ui::GraphicsTool::cb_updateImgObjName(GtkWidget* widget, gpointer data)
{
    imgName =gtk_entry_get_text(GTK_ENTRY(gtkEntryImgName)); // set name //
    updateTreeImgObj(); // update tree thing obj //
}

std::string ui::GraphicsTool::getSizeAsString()
{
    switch (imgFormat)
    {
    case def::IMG_SIZE::IMG_SIZE_32X32: return "32x32";
        break;
    case def::IMG_SIZE::IMG_SIZE_32X64: return "32x64";
        break;
    case def::IMG_SIZE::IMG_SIZE_64X32: return "64x32";
        break;
    case def::IMG_SIZE::IMG_SIZE_64X64: return "64x64";
        break;
    default: return "getSizeAsString() unknown type!";
        break;
    }
}

bool ui::GraphicsTool::loadImgPackFromJson()
{
    std::ifstream ifs("resources//img_pack.json");// file to read //
    Json::CharReaderBuilder rbuilder;	// reader //
    std::string errs; // to check errors //
    Json::Value jsonObj;
    Json::parseFromStream(rbuilder, ifs, &jsonObj, &errs); // parser // 
    
    if (!jsonObj.isNull()) // loading img pack //
    {
        gAuxUI->printMsg("Loading ImgPack from file!");
        for (auto it = jsonObj.begin(); it != jsonObj.end(); it++)
        {
          
            std::vector<math::Vec2<int>> refs;
            refs.push_back(math::Vec2<int>((*it)["0_ref_x"].asInt(), (*it)["0_ref_y"].asInt()));

            std::string name = it.name();
            std::string size = (*it)["size"].asString();
            def::IMG_SIZE imgSize;
            if (size == "32x32") imgSize = def::IMG_SIZE::IMG_SIZE_32X32;
            if (size == "32x64") imgSize = def::IMG_SIZE::IMG_SIZE_32X64;
            if (size == "64x32") imgSize = def::IMG_SIZE::IMG_SIZE_64X32;
            if (size == "64x64") imgSize = def::IMG_SIZE::IMG_SIZE_64X64;
            gResources->getImgPack().addImgObjFromJson(data::ImgObj(name, imgSize, refs));
        }
        // sort structure before display //
        gResources->getImgPack().sort();
        gImgPackUI->updateTree();

    }
    else
    {
        gAuxUI->printMsg("No ImgPack data to be loaded!");
    }
}

GtkWidget* ui::GraphicsTool::getParentFrame()
{
    return gtk_widget_get_parent(GTK_WIDGET(gtkFrameGraphicsTool));
}