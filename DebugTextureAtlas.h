#pragma once
#ifdef TME_DEBUG
#include <gtk/gtk.h>

/*!
 Class DebugTextureAtlas
 =======================

 This class was created only to test the image insertion, image deletion, memory reallocation and data structure reorganization algorithms.
*/
class DebugTextureAtlas
{
public:
	/**
	 * constructor.
	 * 
	 * @param pixlBuff of TextureAtlas for easy viewing of tests.
	 */
	DebugTextureAtlas(GdkPixbuf* pixlBuff);

	/**
	 * rendering function.
	 */
	static gboolean cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data);
	static GtkWidget* drawingArea; /**< drawing area*/
	static cairo_surface_t* surface;  /**< drawing surface*/
	static GObject* gtkMapFrame;  /**< drawing container*/
};
#endif // TME_DEBUG



