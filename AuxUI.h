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

		/* get functions */
		static GtkStatusbar* getStatusBar() { return GTK_STATUSBAR(gtkStatusBar); }

		/* events */
		static void cb_doSearchThing(GtkWidget* widget, gpointer data);
		static void cb_onFocusInEvent(GtkWidget* widget, gpointer data);
		static void cb_onFocusOutEvent(GtkWidget* widget, gpointer data);
		static void cb_onActive(GtkWidget* widget, gpointer data);

	};

}
