#pragma once
#include <gtk/gtk.h>

#include "MapResources.h"
#include "AuxUI.h" // Delet Things
#include "DebugTextureAtlas.h"
#include "ThingCreatorTool.h"
#include "GraphicsTool.h"

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
		static void static_cb_clicked(GtkCellRendererText* renderer, char* path, char* new_text, gpointer user_data);
		void cb_clicked(GtkCellRendererText* renderer, char* path, char* new_text, gpointer user_data);
		std::string imgNameOnActiveRow;
	public:
		ImgPackUI();

		/**
		 *  @brief This method update user interface tree view.
		 */
		static void updateTree();

		/**
		 *  @brief This method removes a Thing from stuffbook dicionary.
		 */
		static gboolean cb_removeImgObj(GtkWidget* widget, GdkEventKey* event, gpointer   user_data);

		/**
		 *  @brief This method select a ImgObj and add it to the Thing creator tool.
		 */
		static void static_cb_selectImg(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data);
		void cb_selectImg(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data);
		
		/**
		 *  @brief This method gets gtkTreeViewImgPack (GTK_TREE_VIEW);
		 */
		static GtkTreeView* getTreeView() { return GTK_TREE_VIEW(gtkTreeViewImgPack); }

		void sort();
	};

}

