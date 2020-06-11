#pragma once

#include <gtk/gtk.h>
#include <string>
#include "Thing.h"
#include "MapResources.h"
#include "StuffBookUI.h"
#include <json/json.h>
#include <iostream>

namespace data { class ImgObj; }

namespace ui {
	/*!
		THING CREATOR MODULE
		====================

		This class represents all graphical interface necessary to create a thing obj.
	 */
	class ThingCreatorTool
	{
	private:
		static GObject* gtkEntryThingName;        /* label to enter the name of thing [creating a new name] */
		//static GObject* gtkEntryThingImg;         /* label to enter the name of img from imgBook */	
		static GObject* gtkTreeViewThingType;     /* shows thing types */
		static GObject* gtkTreeViewThingObj;      /* show thing atributes */
		static GObject* gtkButtonCreateThing;     /* create thing [add to stuffBook] */
		static GObject* gtkFrameThingImgView;     /* gtkFrame */
		

		static TME::Thing thing; // thing structure 

		static GdkPixbuf* pixelRegion;   
		static GdkPixbuf* pixelRegionBackground;		
		static GtkWidget* drawingArea;

		void createTreeViewThingObj();
		void createTreeViewThingType();
		static GtkTreeModel* fillTreeThingObj();
		static GtkTreeModel* fillTreeThingType();

	public:
		/*
		 * constructor
		*/
		ThingCreatorTool();

		/**
		 *  @brief This method update user interface tree view thing obj.
		 */
		static void updateTreeThingObj();

		/**
		 *  @brief This method update user interface tree view thing type.
		 */
		static void updateTreeThingType();

		/**
		 *  @brief This method create a thing, update user interface then select the new thing into the tree view.
		 *  @param widget that will recieve the signal.
		 *  @param data extra information if needed.
		 */
		static void cb_createThing(GtkWidget* widget, gpointer data);

		/**
		 *  @brief This method change a thing name when key are pressed from user interface thing name.
		 *  @param widget that will recieve the signal.
		 *  @param data extra information if needed.
		 */
		static void cb_updateThingName(GtkWidget* widget, gpointer data);

		/**
		 *  @brief This method change a thing type when 2x click into user interface tree view thing type.
		 *  @param widget that will recieve the signal.
		 *  @param data extra information if needed.
		 */
		static void cb_updateThingType(GtkTreeView* tree_view, GtkTreePath* path, GtkTreeViewColumn* column, gpointer user_data);

		/**
		 *  @brief This method sets the Thing ImgObj pointer (data::ImgObj *).
		 */
		static void setThingImgObjPtr(data::ImgObj *);

		/**
		 *  @brief This method represents a redenring callback of Thing ImgObj.
		 */
		static  gboolean cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data);

		/**
		 *  @brief This method copies a GdkPixbuf into this pixel region.
		 */
		void copyPixels(const GdkPixbuf* srcToCopy);
	};
}

