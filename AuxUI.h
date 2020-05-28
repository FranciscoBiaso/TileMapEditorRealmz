#pragma once
#include <gtk/gtk.h>
#include "MapResources.h"
#include "StuffBookUI.h"

namespace ui {
	
	class AuxUI 
	{
	private:
		static GObject* gtkEntryThingSearch;
		static GObject* gtkStatusBar;

	public:
		AuxUI();

		/* get */
		static GtkStatusbar* getStatusBar() { return GTK_STATUSBAR(gtkStatusBar); }

		/* events */
		static void doSearchThing(GtkWidget* widget, gpointer data);
		static void onFocusInEvent(GtkWidget* widget, gpointer data);
		static void onFocusOutEvent(GtkWidget* widget, gpointer data);
		static void onActive(GtkWidget* widget, gpointer data);

	};

}
