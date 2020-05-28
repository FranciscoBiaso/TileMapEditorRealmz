#pragma once

#include "MapResources.h"
#include "AuxUI.h" // Delet Things

#include <gtk/gtk.h>
#include <iostream>
#include <gdk/gdkkeysyms.h> // keyboard del key
#include <algorithm>
#include <map>

namespace ui {
	class StuffBookUI
	{
	private:
		static GObject* gtkTreeViewStuffBook;        // tree model // 
		static GObject* gtkScrolledWindowStuffbook;  // scrolled window // 
	

	public:
		void createTreeView();           // create model //
		static GtkTreeModel* fillTree(); // fill model //
		
		StuffBookUI();

		/* update models */
		static void updateTree();
		static gboolean removeThing(GtkWidget* widget, GdkEventKey* event,gpointer   user_data);

		/* get */
		static GtkTreeView* getTreeViewStuffBook() { return GTK_TREE_VIEW(gtkTreeViewStuffBook); }
	};
}

