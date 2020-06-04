#pragma once
#include <gtk/gtk.h>

#include "MapResources.h"
#include "AuxUI.h" // Delet Things
#include "DebugTextureAtlas.h"

namespace ui {

	/*!
	 class ImgPackUI
	 ===============

	 This class represents the ImgPackUI graphical interface.
	*/
	class ImgPackUI
	{
	private:
		static GObject* gtkTreeViewImgPack;        // tree model //
		static GObject* gtkScrolledWindowImgPack;  // scrolled window //

		void createTreeView();
		static GtkTreeModel* fillTree();
	public:
		ImgPackUI();

		/**
		 *  @brief This method update user interface tree view.
		 */
		static void updateTree();

		/**
		 *  @brief This removes a Thing from stuffbook dicionary.
		 */
		static gboolean cb_removeImgObj(GtkWidget* widget, GdkEventKey* event, gpointer   user_data);
	};

}

