#pragma once

#include "Map.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>

namespace ui {

	/*!
	MapUI class
	===========

	This class represents the map view user interface.
	*/

	class MapUI: public scene::Map
	{
	private:
		 GtkWidget* drawingArea; // widget to draw //
		 GObject* gtkMapFrame; // container to draw the map //

		/**
		 *  @brief Auxiliary function to avoid static members.
		 */
		 static void static_cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data);
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

