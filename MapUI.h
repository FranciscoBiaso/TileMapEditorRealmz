#pragma once

#include "Map.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>

namespace ui {
	class MapUI: public scene::Map
	{
	private:
		 GdkPixbuf* pixelbuf;
		 GtkWidget* drawingArea;
		 GObject* gtkMapFrame;
		 static void static_cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data){
			 reinterpret_cast<MapUI*>(data)->cb_draw_callback(widget,cr,data);
		 }
	public:
		/**
		 * constructor.
		 */
		MapUI(std::string name, int width, int height);

		/**
		 *  @brief This method represents a redenring callback.
		 */
		gboolean cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data);
	};
}

