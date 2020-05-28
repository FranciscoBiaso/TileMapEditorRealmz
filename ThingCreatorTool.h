#pragma once

#include <gtk/gtk.h>
#include <string>
#include "Thing.h"
#include "MapResources.h"
#include "StuffBookUI.h"
#include <json/json.h>
#include <iostream>

enum {
	COLUMN = 0,
	NUM_COLS
};

namespace ui {

	class ThingCreatorTool
	{
	private:
		static GObject* gtkEntryThingName;        /* label to enter the name of thing [creating a new name] */
		static GObject* gtkEntryThingImg;         /* label to enter the name of img from imgBook */	
		static GObject* gtkTreeViewThingType;     /* shows thing types */
		static GObject* gtkTreeViewThingObj;      /* show thing atributes */
		static GObject* gtkButtonCreateThing;     /* create thing [add to stuffBook] */

		static Thing thing; // thing structure 		

	public:
		ThingCreatorTool();
		
		/* create models */
		void createTreeViewThingObj();
		void createTreeViewThingType();

		/* update models */
		static GtkTreeModel* fillTreeThingObj();
		static GtkTreeModel* fillTreeThingType();
		static void updateTreeThingObj();
		static void updateTreeThingType();

		/* create functions */
		static void createThing(GtkWidget* widget, gpointer data);

		/* update functions */
		static void updateThingName();
		static void onGtkEntryThingNameChanged(GtkWidget* widget, gpointer data);
		
		static void updateThingType(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data);

	};
}

