#pragma once

#include "MapResources.h"
#include "AuxUI.h" // Delet Things

#include <gtk/gtk.h>
#include <iostream>
#include <gdk/gdkkeysyms.h> // keyboard del key
#include <algorithm>
#include <map>

namespace ui {
	/*!
		STUFF BOOK 
		==========

		This class represents the stuffbook graphical interface.
	 */
	class StuffBookUI
	{
	private:
		static GObject* gtkTreeViewStuffBook;        // tree model //
		static GObject* gtkScrolledWindowStuffbook;  // scrolled window //
		
		void createTreeView();           // create model //
		static GtkTreeModel* fillTree(); // fill model //
		static void updateTree();        // update user interface //

	public:

		/**
		 *  constructor
		 */
		StuffBookUI();

		/**
		 *  @brief This removes a Thing from stuffbook dicionary.
		 */
		static gboolean removeThing(GtkWidget* widget, GdkEventKey* event, gpointer   user_data);

		/**
		 *  @brief This method gets gtkTreeViewStuffBook.
		 */
		static GtkTreeView* getTreeViewStuffBook() { return GTK_TREE_VIEW(gtkTreeViewStuffBook); }
	};
}

