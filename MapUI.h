#pragma once

#include "Map.h"
#include "Thing.h"
#include "Vec2.h"
#include "CtrlMap.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <iostream>
#include "GLScence.h"

namespace ui {

	/*!
	MapUI class
	===========

	This class represents the map view user interface.
	*/

	class MapUI: protected scene::Map
	{
		enum ctrlModes : int {
			DRAWING_EMPTY = 0,
			DRAWING_PEN_SELECTED = 1,
			DRAWING_ERASER_SELECTED,
			MOVING_VIEW_OF_MAP
		};
	private:
		GLScence* _glScene;
		GtkWidget* drawingArea; // widget to draw //
		GObject* gtkMapViewPort; // container to draw the map //
		GObject* gtkMapFrame; 
		GObject* scrolledwindowMapUI;
		GObject* _gtk_label_mouse_coords;
		int viewWidth, viewHeight;
		int _map_layer;
		data::Thing drawObj;
		math::Vec2<int> mousePosition_windows_coord;
		math::Vec2<int> mousePosition_by_32;
		math::Vec2<int> mousePosition_select_from;
		math::Vec2<int> mousePosition_select_to;
		
		math::Vec2<int> mousePositionPrevious;
		math::Vec2<int> mouseStartPositionToMoveMapView; /**< position of mouse when keyboard space was pressed */
		math::Vec2<int> mapDetachment; /**< displace of mouseStartPositionToMoveMapView and new mouse position */
		math::Vec3<float> gridColor;
		math::Vec2<int> _view_center;
		math::Vec2<int> _camera_position;  /**< center of camera */
		math::Vec2<int> _camera_position_when_user_press_space;  /**<  */

		glm::vec2 _mouse_to_world;
		glm::vec2 _mouse_to_world_previous;
		glm::vec2 _camera_on_hit_space;
		glm::vec2 _mouse_coord;
		glm::vec2 _mousePosition_select_from;
		glm::vec2 _mousePosition_select_to;

		int _camera_delta; /**< camera displacement (x,y) */
		double _camera_move_speed; /**< camera speed when moving with space key */
		bool canDrawMouseShadowSquare;
		bool _canDrawSelectionSquare;
		bool thingIsSelected;
		bool _draw_map_borders;
		int ctrlModes;
		int ctrlModesPrevious;
		GdkPixbuf* cursorPixelbuf;
		GdkPixbuf* _pixelbuf_unity_grid;
		static GdkPixbuf* _pixelbuf_full_Grid;  // pixelbuff used to draw grid //
		cairo_surface_t* _surface_grid;
		bool _grid_enable;
		int _scroll_x_position;
		int _scroll_y_position;

		math::Vec2<int> screen_coords_to_world_coords(math::Vec2<int>);

		void draw_map_ui(cairo_t* cr);

		void hide_shadow_square();
		void show_shadow_square();

		// mouse zoom //
		bool isLeftKeyPressed;



		/**
		 *  @brief Do the inverse operation of drawing tool operations.
		 *  Example 1: AddThing -> DoInverse -> RemThing.
		 *  Example 2: RemThing -> DoInverse -> AddThing.
		 *  This function is created to be used with ctrl+z/ctrl+y functionalities.
		 */
		void do_reverse_operation(ctrl::sOperation operation);

		/**
		 *  @brief gets variable map borders.
		 */
		bool can_draw_map_borders();

		/**
		 *  @brief block the limite of camera.
		 */
		void camera_block();
		/**
		 *  @brief Get camera position.
		 */
		math::Vec2<int> camera_get_position();
		/**
		 *  @brief Change the value of delta variable.
		 */
		void camera_set_delta(int);
		/**
		 *  @brief Increase the value of delta by value.
		 */
		void camera_increment_delta(int value);
		/**
		 *  @brief Decrease the value of delta by value.
		 */
		void camera_decrement_delta(int);
		/**
		 *  @brief Move camera center position to the right with delta displacemnt.
		 */
		void camera_move_right();
		/**
		 *  @brief Move camera center position to the top with delta displacemnt.
		 */
		void camera_move_top();
		/**
		 *  @brief Move camera center position to the bottom with delta displacemnt.
		 */
		void camera_move_bottom();
		/**
		 *  @brief Move camera center position to the left with delta displacemnt.
		 */
		void camera_move_left();

		/**
		 *  @brief Auxiliary function to avoid static members.
		 */
		static gboolean static_cb_scroll(GtkWidget* widget, GdkEvent* event, gpointer user_data);

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

		void loadOpenGLMap();
	public:
		/**
		 * constructor.
		 */
		MapUI(std::string name, int width, int height, int levels);



		gboolean cb_scroll(GtkWidget* widget, GdkEvent* event, gpointer user_data);

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

		data::Thing  addThingMapUI(math::Vec2<int> world_coords);
		void delThingMapUI(math::Vec2<int> world_coords);
		void delThingMapUI(std::string thing_name,math::Vec3<int> thing_position);
		void delThingMapUI(data::Thing*);


		data::Thing  addThingMapUI(glm::vec2 world_coords);
		void delThingMapUI(glm::vec2 world_coords);

		data::Thing addThingMapUI(int line, int col);
		void delThingMapUI(int line, int col);

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
		 * @brief This method centralize camera position.
		 */
		void camera_at(math::Vec2<int> position);
		/**
		 * @brief This method moves the camera center positon by (dx,dy).
		 */
		void camera_move(int dx, int dy);

		/**
		 * @brief This method enable draw map borders.
		 */
		void enable_draw_map_borders();
		/**
		 * @brief This method disable draw map borders.
		 */
		void disable_draw_map_borders();

		/**
		 *  @brief converts mouse coords to words position as a string.
		 */
		std::string mouse_coords_to_word_position_to_string(math::Vec2<int> screen_coord);
		
		void loadMapFromJson();
		void saveMap() { this->saveInternalMap(); }

		GLScence* getGLScene() { return _glScence; }
	};
}

