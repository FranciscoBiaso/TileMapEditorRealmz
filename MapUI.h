#pragma once

#include "Map.h"
#include "Thing.h"
#include "Vec2.h"
#include "CtrlMap.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <iostream>
namespace ui {

	/*!
	MapUI class
	===========

	This class represents the map view user interface.
	*/

	class MapUI: public scene::Map
	{
		enum ctrlModes : int {
			DRAWING_EMPTY = 0,
			DRAWING_PEN_SELECTED = 1,
			DRAWING_ERASER_SELECTED,
			MOVING_VIEW_OF_MAP
		};
	private:
		 GtkWidget* drawingArea; // widget to draw //
		 GObject* gtkMapViewPort; // container to draw the map //
		 GObject* gtkMapFrame; 
		 GObject* scrolledwindowMapUI;
		 int viewWidth, viewHeight;
		 int _map_layer;
		 data::Thing drawObj;
		 math::Vec2<int> mousePosition;
		 math::Vec2<int> mousePositionPrevious;
		 math::Vec2<int> mouseStartPositionToMoveMapView;
		 math::Vec2<int> mapDetachment;
		 math::Vec3<float> gridColor;
		 math::Vec2<int> _view_center;
		 math::Vec2<int> _camera_position;
		 bool canDrawMouseShadowSquare;
		 bool thingIsSelected;
		 int ctrlModes;
		 int ctrlModesPrevious;
		 GdkPixbuf* cursorPixelbuf;
		 GdkPixbuf* _pixelbuf_unity_grid;
		 static GdkPixbuf* _pixelbuf_full_Grid;  // pixelbuff used to draw grid //
		 cairo_surface_t* _surface_grid;
		 bool _grid_enable;
		 int _scroll_x_position;
		 int _scroll_y_position;

		/**
		 *  @brief Auxiliary function to avoid static members.
		 */
		 static void static_cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data);

		 /**
		  *  @brief Auxiliary function to avoid static members.
		  */
		 static gboolean static_cb_clickNotify(GtkWidget* widget, GdkEvent* event, gpointer user_data);

		 /**
		  *  @brief Auxiliary function to avoid static members.
		  */
		 static gboolean static_cb_MotionNotify(GtkWidget* widget, GdkEventMotion* event, gpointer user_data);

		 /**
		  *  @brief Auxiliary function to avoid static members.
		  */
		 static gboolean static_cb_onEnter(GtkWidget* widget, GdkEvent* event, gpointer user_data);

		 /**
		  *  @brief Auxiliary function to avoid static members.
		  */
		 static gboolean static_cb_onLeave(GtkWidget* widget, GdkEvent* event, gpointer user_data);

	public:
		/**
		 * constructor.
		 */
		MapUI(std::string name, int width, int height);

		/**
		 *  @brief This method represents a redenring callback.
		 */
		gboolean cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data);

		/**
		 * @brief This method add a Thing into the map where mouse is positioned, when 1x left - click.
		 */
		gboolean cb_clickNotify(GtkWidget* widget, GdkEvent* event, gpointer user_data);

		/**
		 * @brief This method selects a Thing to be drawn into the map.
		 */
		void setDrawThingObj(data::Thing it);

		/**
		 * @brief This method selects a Thing to be drawn into the map.
		 */
		gboolean cb_MotionNotify(GtkWidget* widget, GdkEventMotion* event, gpointer user_data);

		// w i p //
		static void static_my_getsize(GtkWidget* widget, GtkAllocation* allocation, void* data);

		// w i p //
		void map_resize(GtkWidget* widget, GtkAllocation* allocation, void* data);

		data::Thing  addThingMapUI();
		void delThingMapUI();
		void delThingMapUI(std::string thing_name,math::Vec3<int> thing_position);
		void delThingMapUI(data::Thing*);

		/**
		 * @brief This method is active when mouse enters into this user interface.
		 */
		gboolean cb_onEnter(GtkWidget* widget,GdkEvent* event,gpointer user_data);

		/**
		 * @brief This method is active when mouse leaves from this user interface.
		 */
		gboolean cb_onLeave(GtkWidget* widget, GdkEvent* event, gpointer user_data);

		/**
		 * @brief This method selects a new cursor to represent the drawing tool.
		 */
		void selectCursor();

		/**
		 * @brief This method delets calls internal delet all things.
		 */
		void deletAllThingsFromTheMap(std::string thingName);

		/**
		 * @brief This method force redraws the entire map user interface.
		 */
		void forceRedraw();

		/**
		 * @brief This method move map view.
		 */
		void updateMapView();

		void do_reverse_operation(ctrl::sOperation operation);

		void value_change(GtkAdjustment* adjustment,gpointer user_data);

		void move_camera(math::Vec2<int> position);
	};
}

