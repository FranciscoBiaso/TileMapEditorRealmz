#include "GraphicsTool.h"

GObject* ui::GraphicsTool::gtkFileChooserButtonImg = nullptr;
GObject* ui::GraphicsTool::gtkImgInput = nullptr;
GObject* ui::GraphicsTool::gtkFrameImg = nullptr;
GObject* ui::GraphicsTool::gtkFrameImgView = nullptr;
GObject* ui::GraphicsTool::gtkEntryImgName = nullptr;
GObject* ui::GraphicsTool::gtkButtonAddImgName = nullptr;
GObject* ui::GraphicsTool::gtkTreeViewImgObj = nullptr;
GObject* ui::GraphicsTool::gtkButtonCreateImg = nullptr;
GObject* ui::GraphicsTool::gtkScrolledWindowTileset = nullptr;
GObject* ui::GraphicsTool::gtkCheckButtonGrid = nullptr;
GObject* ui::GraphicsTool::gtkToggleButton32x32 = nullptr;
GObject* ui::GraphicsTool::gtkToggleButton32x64 = nullptr;
GObject* ui::GraphicsTool::gtkToggleButton64x32 = nullptr;
GObject* ui::GraphicsTool::gtkToggleButton64x64 = nullptr;

GtkWidget* ui::GraphicsTool::drawingArea = nullptr;
GtkWidget* ui::GraphicsTool::drawingAreaImgObj = nullptr;
GdkPixbuf* ui::GraphicsTool::pixelBuf = nullptr;
GdkPixbuf* ui::GraphicsTool::pixelBufImgDest = nullptr; // img pixel buf //

int ui::GraphicsTool::xSqr = 0;
int ui::GraphicsTool::ySqr = 0;
int ui::GraphicsTool::imgCursor = 0;
bool ui::GraphicsTool::canDrawSelectedSquare = false;

cairo_surface_t*  ui::GraphicsTool::surface = nullptr;

namespace GtkUserInterface {
    extern GtkBuilder* builder;
}

ui::GraphicsTool::GraphicsTool()
{
    // initialize timer //
    g_timeout_add(600, timer_changeSquareData, NULL);

    gtkFileChooserButtonImg = gtk_builder_get_object(GtkUserInterface::builder, "gtkFileChooserButtonImg");
    gtkImgInput = gtk_builder_get_object(GtkUserInterface::builder, "gtkImgInput");
    gtkFrameImg = gtk_builder_get_object(GtkUserInterface::builder, "gtkFrameImg");
    gtkFrameImgView = gtk_builder_get_object(GtkUserInterface::builder, "gtkFrameImgView");
    gtkCheckButtonGrid = gtk_builder_get_object(GtkUserInterface::builder, "gtkCheckButtonGrid");
    /* toggle buttons */
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
    g_signal_connect(gtkToggleButton32x32, "toggled", G_CALLBACK(signalGtkToggleButton32x32), gtkToggleButton32x32);
    g_signal_connect(gtkToggleButton32x64, "toggled", G_CALLBACK(signalGtkToggleButton32x64), gtkToggleButton32x64);
    g_signal_connect(gtkToggleButton64x32, "toggled", G_CALLBACK(signalGtkToggleButton64x32), gtkToggleButton64x32);
    g_signal_connect(gtkToggleButton64x64, "toggled", G_CALLBACK(signalGtkToggleButton64x64), gtkToggleButton64x64);

    if (drawingAreaImgObj == nullptr)
    {
        drawingAreaImgObj = gtk_drawing_area_new();
        //gtk_widget_add_events(drawingArea, GDK_POINTER_MOTION_MASK);
         gtk_container_add(GTK_CONTAINER(gtkFrameImgView), drawingAreaImgObj);

        g_signal_connect(G_OBJECT(drawingAreaImgObj), "draw", G_CALLBACK(draw_callback_img_obj), NULL);

        /* grab motion event */
        //g_signal_connect(G_OBJECT(drawingArea), "motion-notify-event", G_CALLBACK(motion_notify_event_cb), NULL);
    }

    // alocate memory for img pixel buf
    pixelBufImgDest = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, REALMZ_GRID_SIZE * 2, REALMZ_GRID_SIZE * 2);

    //gtkScrolledWindowTileset = gtk_builder_get_object(GtkUserInterface::builder, "gtkScrolledWindowTileset");

    std::string path = g_win32_get_package_installation_directory_of_module(NULL);
    path += "\\imgs\\";
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(gtkFileChooserButtonImg), path.c_str());
    gtk_file_chooser_set_action(GTK_FILE_CHOOSER(gtkFileChooserButtonImg), GTK_FILE_CHOOSER_ACTION_OPEN);
    //gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(gtkFileChooserButtonImg), TRUE);

    //gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(gtkFileChooserButtonImg), true);
    g_signal_connect(gtkFileChooserButtonImg, "file-set", G_CALLBACK(loadImg), gtk_builder_get_object(GtkUserInterface::builder, "viewport1"));

    /* bind check button to grid action (show or hide) */
    g_signal_connect(gtkCheckButtonGrid, "toggled", G_CALLBACK(toggleButtonGrid), NULL);

}


gboolean ui::GraphicsTool::motion_notify_event_cb(GtkWidget* widget, GdkEventMotion* e, gpointer   user_data)
{
    xSqr = (e->x/REALMZ_GRID_SIZE);
    xSqr = (int)xSqr * REALMZ_GRID_SIZE;
    ySqr = (e->y / REALMZ_GRID_SIZE) ;
    ySqr = (int)ySqr * REALMZ_GRID_SIZE;
    if (drawingArea != nullptr)
        gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
    return TRUE;    
}


gboolean ui::GraphicsTool::click_event_cb(GtkWidget* widget, GdkEventButton* event, gpointer   user_data)
{
    // double click //
    if (event->type == GDK_DOUBLE_BUTTON_PRESS)
    {      
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32)) == TRUE)
        {
            gdk_pixbuf_copy_area(pixelBuf, xSqr, ySqr, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2);
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE)
        {
            gdk_pixbuf_copy_area(pixelBuf, xSqr, ySqr, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, REALMZ_GRID_SIZE / 2,imgCursor * REALMZ_GRID_SIZE);
            rightShiftCursor();
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
        {
            gdk_pixbuf_copy_area(pixelBuf, xSqr, ySqr, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, imgCursor * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE / 2);
            rightShiftCursor();
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
        {
            if(imgCursor <= 1)
                gdk_pixbuf_copy_area(pixelBuf, xSqr, ySqr, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, imgCursor * REALMZ_GRID_SIZE, 0);
            else
                gdk_pixbuf_copy_area(pixelBuf, xSqr, ySqr, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBufImgDest, (imgCursor-2) * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE);
            rightShiftCursor();
        }

        gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgObj));
    }

    return TRUE;
}

/* show or hide grid */
void ui::GraphicsTool::toggleButtonGrid(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (drawingArea != nullptr)
        gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
}

gboolean ui::GraphicsTool::draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    guint width, height;
    GdkRGBA color; 
    GtkStyleContext* context;    
    float scaleFactor = 1;

    cairo_set_source_rgba(cr, 0.5,0.5,0.5,1);
    cairo_paint(cr);    

    width = gtk_widget_get_allocated_width(widget);
    height = gtk_widget_get_allocated_height(widget);
   
    cairo_scale(cr, scaleFactor, scaleFactor);

    //gdk_pixbuf_copy_area()
    
    int w = cairo_image_surface_get_width(surface);
    int h = cairo_image_surface_get_height(surface);

    cairo_set_source_surface(cr, surface, 0,0);
    cairo_paint(cr);

    draw_square(cr, xSqr, ySqr, 0.1, 0.1, 0.85, 0.35);

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkCheckButtonGrid)) == TRUE)
        draw_grid(cr, width, height, REALMZ_GRID_SIZE);

    return FALSE;
}

gboolean ui::GraphicsTool::draw_callback_img_obj(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    guint width, height;
    GdkRGBA color;
    GtkStyleContext* context;
    float scaleFactor = 1;

    cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.2);
    cairo_paint(cr);

    width = gtk_widget_get_allocated_width(widget);
    height = gtk_widget_get_allocated_height(widget);

    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32)) == TRUE)
    {
        drawImg32x32(cr);
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE)
    {
        drawImg32x64(cr);
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
    {
        drawImg64x32(cr);
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
    {
        drawImg64x64(cr);
    }
    

    if (pixelBufImgDest != nullptr)
    {
        cairo_surface_t* surface = gdk_cairo_surface_create_from_pixbuf(pixelBufImgDest, 0, NULL);
        cairo_set_source_surface(cr, surface, 0, 0);
        cairo_paint(cr);
    }

    // highligh square //
    if (canDrawSelectedSquare)
    {
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32)) == TRUE)
        {
            draw_square(cr, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2, 0,1,0, 0.2);
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64)) == TRUE)
        {
            draw_square(cr, REALMZ_GRID_SIZE / 2, imgCursor * REALMZ_GRID_SIZE, 0, 1, 0, 0.2);
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32)) == TRUE)
        {
            draw_square(cr, imgCursor * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE / 2, 0, 1, 0, 0.2);
        }
        else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64)) == TRUE)
        {
            if (imgCursor <= 1)
                draw_square(cr, imgCursor * REALMZ_GRID_SIZE, 0, 0, 1, 0, 0.2);
            else
                draw_square(cr, (imgCursor - 2) * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, 0, 1, 0, 0.2);
        }
    }
    return FALSE;
}

void ui::GraphicsTool::loadImg(GtkFileChooserButton* widget, gpointer data)
{    

    // create the are we can draw in
    //GtkDrawingArea* drawingArea;

    pixelBuf = gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gtkFileChooserButtonImg)))));
    surface = gdk_cairo_surface_create_from_pixbuf(pixelBuf, 0, NULL);
    
    if (drawingArea == nullptr)
    {
        drawingArea = gtk_drawing_area_new();
        gtk_widget_add_events(drawingArea, GDK_POINTER_MOTION_MASK);
        gtk_widget_add_events(drawingArea, GDK_BUTTON_PRESS_MASK);

        gtk_container_add(GTK_CONTAINER(data), drawingArea);

        gtk_widget_set_size_request(GTK_WIDGET(drawingArea), gdk_pixbuf_get_width(pixelBuf), gdk_pixbuf_get_height(pixelBuf));

        g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(draw_callback), NULL);

        /* grab motion event */
        g_signal_connect(G_OBJECT(drawingArea), "motion-notify-event", G_CALLBACK(motion_notify_event_cb), NULL);
        g_signal_connect(G_OBJECT(drawingArea), "button-press-event", G_CALLBACK(click_event_cb), NULL);


        gtk_widget_show_all(GTK_WIDGET(gtk_widget_get_parent(drawingArea)));        
    }
   
    //gtk_widget_set_size_request(drawingArea, gdk_pixbuf_get_width(pixelBuf), gdk_pixbuf_get_height(pixelBuf));
    gtk_widget_set_size_request(GTK_WIDGET(gtkFrameImg), std::min(gdk_pixbuf_get_width(pixelBuf),MAX_IMG_WIDGET_WIDTH), std::min(gdk_pixbuf_get_height(pixelBuf),MAX_IMG_WIDGET_HEIGHT));
    
    
    gtk_widget_show_all(GTK_WIDGET(gtk_widget_get_parent(drawingArea)));
    //g_signal_connect(drawingArea, "configure-event",G_CALLBACK(configure_event_cb), NULL);

}

/* toggle buttons signals */

void ui::GraphicsTool::signalGtkToggleButton32x32(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gdk_pixbuf_fill(pixelBufImgDest, 0x00000005);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64), FALSE);
        gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgObj));
    }
}

void ui::GraphicsTool::signalGtkToggleButton32x64(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gdk_pixbuf_fill(pixelBufImgDest, 0x00000005);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64), FALSE);
        gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgObj));
    }
}

void ui::GraphicsTool::signalGtkToggleButton64x32(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gdk_pixbuf_fill(pixelBufImgDest, 0x00000005);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x64), FALSE);
        gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgObj));
    }
}

void ui::GraphicsTool::signalGtkToggleButton64x64(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton) == TRUE)
    {
        gdk_pixbuf_fill(pixelBufImgDest, 0x00000005);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x32), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton32x64), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkToggleButton64x32), FALSE);
        gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgObj));
    }
}

/* DRAWING FUNCTIONS */
void ui::GraphicsTool::draw_grid(cairo_t* cr, int w, int h, int gridSize)
{
    // grid basic settings //

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

void ui::GraphicsTool::draw_square(cairo_t* cr, int x, int y, float r, float g , float b, float a)
{
    //cairo_set_line_width(cr, 2);
    //cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
    
    cairo_rectangle(cr, x, y,REALMZ_GRID_SIZE,REALMZ_GRID_SIZE);
    cairo_set_source_rgba(cr, r, g, b, a);
    cairo_fill(cr);
}

void ui::GraphicsTool::drawImg32x32(cairo_t* cr)
{
    /* draw on center */
    draw_square(cr, REALMZ_GRID_SIZE/2, REALMZ_GRID_SIZE / 2, 0.2, 0.2, 0.2, 0.5);
}


void ui::GraphicsTool::drawImg32x64(cairo_t* cr)
{
    draw_square(cr, REALMZ_GRID_SIZE / 2, 0, 0.2, 0.2, 0.2, 0.5);
    draw_square(cr, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, 0.2, 0.2, 0.2, 0.5);
}

void ui::GraphicsTool::drawImg64x32(cairo_t* cr)
{
    draw_square(cr, 0, REALMZ_GRID_SIZE / 2, 0.2, 0.2, 0.2, 0.5);
    draw_square(cr, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE / 2, 0.2, 0.2, 0.2, 0.5);
}

void ui::GraphicsTool::drawImg64x64(cairo_t* cr)
{
    draw_square(cr, 0, 0, 0.2, 0.2, 0.2, 0.5);
    draw_square(cr, 0, REALMZ_GRID_SIZE, 0.2, 0.2, 0.2, 0.5);
    draw_square(cr, REALMZ_GRID_SIZE, 0, 0.2, 0.2, 0.2, 0.5);
    draw_square(cr, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, 0.2, 0.2, 0.2, 0.5);
}



/* logical */
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

/* timer functions */
gboolean ui::GraphicsTool::timer_changeSquareData(gpointer data)
{
    if (canDrawSelectedSquare)
        canDrawSelectedSquare = false;
    else
        canDrawSelectedSquare = true;
    gtk_widget_queue_draw(GTK_WIDGET(drawingAreaImgObj));

    return TRUE;
}