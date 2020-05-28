#pragma once
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gmodule.h> // glist
#include <gio/gio.h> //gFileInfo
#include <string>
#include <iostream>
#include <algorithm> // max

#define MAX_IMG_WIDGET_WIDTH 320
#define MAX_IMG_WIDGET_HEIGHT 320
#define REALMZ_GRID_SIZE 32

namespace ui {

	class GraphicsTool
	{
	private:
		static GObject* gtkFileChooserButtonImg;
		static GObject* gtkImgInput;
		static GObject* gtkFrameImg;
		static GObject* gtkFrameImgView;
		static GObject* gtkCheckButtonGrid;
		static GObject* gtkEntryImgName;
		static GObject* gtkButtonAddImgName;
		static GObject* gtkTreeViewImgObj;
		static GObject* gtkButtonCreateImg;		
		static GObject* gtkScrolledWindowTileset;
		static GObject* gtkToggleButton32x32;
		static GObject* gtkToggleButton32x64;
		static GObject* gtkToggleButton64x32;
		static GObject* gtkToggleButton64x64;

		static GtkWidget* drawingArea;       // region to display uploaded img //
		static GtkWidget* drawingAreaImgObj; // region to display selected img from uploaded img //
		static GdkPixbuf* pixelBuf;          // buffer of upload img //
		static GdkPixbuf* pixelBufImgDest;   // buffer of selected img //
		static cairo_surface_t* surface;

		static int xSqr;  // position x of mouse into selected img region multiple by GRID_SIZE //
		static int ySqr;  // position y of mouse into selected img region multiple by GRID_SIZE //	 	

		static int imgCursor; // cursor to identify region of selected img region, 4 possibilities //
		static bool canDrawSelectedSquare; // used to draw flashing square //
	public:
		GraphicsTool();
		static void loadImg(GtkFileChooserButton* widget, gpointer data);
		static void toggleButtonGrid(GtkToggleButton* togglebutton, gpointer user_data);

		/* draw functions */
		static void draw_grid(cairo_t* cr, int w, int h, int gridSize);
		static void draw_square(cairo_t* cr, int x, int y, float r = 0, float g = 0, float b = 1, float a = 1);
		static void drawImg32x32(cairo_t* cr);
		static void drawImg32x64(cairo_t* cr);
		static void drawImg64x32(cairo_t* cr);
		static void drawImg64x64(cairo_t* cr);
		
		/* callbacks */
		static gboolean draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data);
		static gboolean draw_callback_img_obj(GtkWidget* widget, cairo_t* cr, gpointer data);

		/* events */
		static gboolean motion_notify_event_cb(GtkWidget* widget, GdkEventMotion* event, gpointer   user_data);
		static gboolean click_event_cb(GtkWidget* widget, GdkEventButton* event, gpointer   user_data);

		/* logic interface functions */
		static void rightShiftCursor();
		static void leftShiftCursor();

		/* timer functions */
		static gboolean timer_changeSquareData(gpointer data);

		/* toggle button signals */
		static void signalGtkToggleButton32x32(GtkToggleButton* togglebutton, gpointer user_data);
		static void signalGtkToggleButton32x64(GtkToggleButton* togglebutton, gpointer user_data);
		static void signalGtkToggleButton64x32(GtkToggleButton* togglebutton, gpointer user_data);
		static void signalGtkToggleButton64x64(GtkToggleButton* togglebutton, gpointer user_data);
		
	};

};

