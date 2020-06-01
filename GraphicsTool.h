#pragma once
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gmodule.h> // glist
#include <gio/gio.h> //gFileInfo
#include <string>
#include <iostream>
#include <algorithm> // max
#include "Definitions.h"
namespace ui {
	/*!
		Graphicas module
		================

		This class is modeled to create the image dictionary. That will contain all the images that will be used in the creation of the Thing objects.
	 */
	class GraphicsTool
	{
		enum FileReturnMsg{
			FILE_OK,
			NO_FILE_iS_SELECTED,
			IMAGE_IS_EMPTY
		};

		static int MAX_IMG_WIDGET_WIDTH;
		static int MAX_IMG_WIDGET_HEIGHT;
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
		static GObject* gtkViewportImgSrc;

		static GtkWidget* drawingAreaImgSrc; // region to display uploaded img //
		static GtkWidget* drawingAreaImgDst; // region to display selected img from uploaded img //
		static GdkPixbuf* pixelBufImgSrc;    // buffer of upload img //
		static GdkPixbuf* pixelBufImgDest;   // buffer of selected img //
		static GdkPixbuf* dragIcon32x32;     // drag icon //
		static cairo_surface_t* surfaceScr;
		static cairo_surface_t* surfaceDst;

		static int xPosHighlightSquare;  // position x of mouse into selected img region multiple by GRID_SIZE //
		static int yPosHighlightSquare;  // position y of mouse into selected img region multiple by GRID_SIZE //

		static int xPosDropSquare;  // position x of mouse when drop img from img scr //
		static int yPosDropSquare;  // position y of mouse when drop img from img scr //

		static int imgCursor; // cursor to identify region of selected img region, 4 possibilities //
		static bool canDrawSelectedSquare; // used to draw flashing square //
		
		static GtkTargetEntry dragTarget;
	public:
		/**
		 *  constructor
		 */
		GraphicsTool();
		
		/**
		 *  @brief This method loads an image from the selected file and stores it in GdkPixbuf.
		 */
		static FileReturnMsg loadImgFromFile();

		/**
		 *  @brief This method set src surface for drawing from src GdkPixbuf;
		 */
		static void setSrcSurfaceFromScrPixelbuf();

		/**
		 *  @brief This method set dst surface for drawing from dst GdkPixbuf;
		 */
		static void setDstSurfaceFromDstPixelbuf();

		/**
		 *  @brief This method sets widget drawingArea for src image.
		 *  @param widget is a container where drawing area will be stored.
		 */
		static void setDrawingAreaImgScr(GtkWidget*widget);

		/**
		 *  @brief This method sets widget drawingArea for dst image.
		 *  @param widget is a container where drawing area will be stored.
		 */
		static void setDrawingAreaImgDst(GtkWidget* widget);

		/**
		 *  @brief This method load image from selected file.
		 *  @param widget that will recieve the signal.
		 */
		static void cb_onFileSet(GtkFileChooserButton* widget, gpointer data);

		/**
		 *  @brief This method change grid to on/off.
		 *  @param widget that will recieve the signal.
		 *  @param data extra information if needed.
		 */
		static void cb_toggleButtonChangeGrid(GtkToggleButton* togglebutton, gpointer data);

		/**
		 *  @brief This method draws a grid lines.
		 *  @param cr surface to draw.
		 *  @param width to draw.
		 *  @param height to draw.
		 *  @param gridSize the space between grid lines.
		 */
		static void drawGrid(cairo_t* cr, int width, int height, int gridSize);

		/**
		 *  @brief This method draws a simples square.
		 *  @param cr surface to draw.
		 *  @param x position.
		 *  @param y position.
		 *  @param w width.
		 *  @param h height.
		 *  @param color RGBA color.
		 */
		static void drawSquare(cairo_t* cr, int x, int y, int w = REALMZ_GRID_SIZE, int h = REALMZ_GRID_SIZE, GdkRGBA color = { 1,1,1,1 });
		
		/**
		 *  @brief This method draws a 32x32 background img centered.
		 */
		static void drawImg32x32(cairo_t* cr, GdkRGBA color = { 1,1,1,1 });

		/**
		 *  @brief This method draws a 32x64 background img centered.
		 */
		static void drawImg32x64(cairo_t* cr, GdkRGBA color = { 1,1,1,1 });

		/**
		 *  @brief This method draws a 64x32 background img centered.
		 */
		static void drawImg64x32(cairo_t* cr, GdkRGBA color = { 1,1,1,1 });

		/**
		 *  @brief This method draws a 64x64 background img centered.
		 */
		static void drawImg64x64(cairo_t* cr, GdkRGBA color = { 1,1,1,1 });
		
		/**
		 *  @brief This method represents a redenring callback of src img widget region.
		 */
		static gboolean cb_draw_callback_img_src(GtkWidget* widget, cairo_t* cr, gpointer data);
		
		/**
		 *  @brief This method represents a redenring callback of dst img widget region.
		 */
		static gboolean cb_draw_callback_img_dst(GtkWidget* widget, cairo_t* cr, gpointer data);

		/**
		 *  @brief This method detects mouse movement and adjust the position values (x, y) to draw the highlighted square.
		 */
		static gboolean cb_MotionNotify(GtkWidget* widget, GdkEventMotion* event, gpointer   user_data);		

		/**
		 * @brief This method copy the 32x32 region of src img where mouse is positioned into sub square (32x32) region of dst img when 2x click.
		 */
		static gboolean cb_clickNotify(GtkWidget* widget, GdkEventButton* event, gpointer   user_data);

		/**
		 * @brief This method shifts the image cursor position to the right.
		 */
		static void rightShiftCursor();

		/**
		 * @brief This method shifts the image cursor position to the left.
		 */
		static void leftShiftCursor();

		/**
		 * @brief This method resets the image cursor position.
		 */
		static void resetCursor();

		/**
		 * @brief This method sets the image cursor to a specific position.
		 * @param x mouse position.
		 * @param y mouse position.
		 */
		static void setCursor(int x, int y);

		/**
		 * @brief This method it is called repeatedly in milliseconds to manipulate highlight square.
		 */
		static gboolean timerChangeSquareData(gpointer data);

		/**
		 * @brief This method occurs when the toggle button 32x32 is triggered.
		 */
		static void cb_signalGtkToggleButton32x32(GtkToggleButton* togglebutton, gpointer user_data);
		
		/**
		 * @brief This method occurs when the toggle button 32x64 is triggered.
		 */

		static void cb_signalGtkToggleButton32x64(GtkToggleButton* togglebutton, gpointer user_data);
		/**
		 * @brief This method occurs when the toggle button 64x632 is triggered.
		 */
		static void cb_signalGtkToggleButton64x32(GtkToggleButton* togglebutton, gpointer user_data);

		/**
		 * @brief This method occurs when the toggle button 64x64 is triggered.
		 */
		static void cb_signalGtkToggleButton64x64(GtkToggleButton* togglebutton, gpointer user_data);

		/**
		 * @brief This method is used to configure the icon to be dragged.
		 */
		static void cb_dragBegin(GtkWidget* widget, GdkDragContext* context,gpointer user_data);

		/**
		 * @brief This method is used to change the target image with the pre-configured icon.
		 */
		static gboolean cb_dragDrop(GtkWidget* widget, GdkDragContext* context,
			gint            x,
			gint            y,
			guint           time,
			gpointer        user_data);

	};

};

