#pragma once

#include <gtk/gtk.h>
#include <string>
#include "json/json.h"
#include <fstream>

namespace ui {

	class ScriptUI
	{
	public:
		ScriptUI();
		void updateTree();

		/**
		 *  @brief Auxiliary function to avoid static members.
		 */
		//static void static_cb_selectThing(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeV

		static void static_cb_rowActive(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data);		
		static void static_cb_onActive(GtkWidget* widget, gpointer data);
		static gboolean static_cb_removeImgObj(GtkWidget* widget, GdkEventKey* event, gpointer   user_data);
		static void static_cb_clicked(GtkCellRendererText* renderer, char* path, char* new_text, gpointer user_data);
		static void static_cb_signalGtkToggleButtonScriptText(GtkToggleButton* togglebutton, gpointer user_data);
		static void static_cb_signalGtkToggleButtonScriptRects(GtkToggleButton* togglebutton, gpointer user_data);
		static gboolean cb_onFocusInEvent(GtkWidget* widget, GdkEvent* event, gpointer   user_data);
		void activeButtons();

		void searchScriptByID(std::string ID);
		void saveScriptsAsJson();
		void loadScriptsFromJson();
	private:
		void createTreeView();
		static GtkTreeModel* fillTree(); // fill model //
		static GObject* gtkTreeViewScript;        // tree model //
		static GObject* gtkScrolledWindowScript;  // scrolled window //
		static GObject* gtkToggleButtonScriptText; // hide and show script texts //
		static GObject* gtkToggleButtonScriptRects; // hide and show script rects //
		static GObject* gtkEntryScriptSearch; //search script //

		void cb_rowActive(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data); // activ click on row //
		void cb_onActive(GtkWidget* widget, gpointer data); // activ enter key //
		gboolean cb_removeImgObj(GtkWidget* widget, GdkEventKey* event, gpointer   user_data);
		void clicked(GtkCellRendererText* renderer, char* path, char* new_text, gpointer user_data);
		void cb_signalGtkToggleButtonScriptText(GtkToggleButton* togglebutton, gpointer user_data);
		void cb_signalGtkToggleButtonScriptRects(GtkToggleButton* togglebutton, gpointer user_data);
	};

};


