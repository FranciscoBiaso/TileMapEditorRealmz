#pragma once
#include "Definitions.h"
#include <gtk/gtk.h>

namespace ui {
	/*!
		Drawing Tool Module
		===================

		This class represents all drawing tool user interface.
	 */

	class DrawingToolUI
	{
	private:
		GObject* brush;
		GObject* eraser;
		GObject* zoomIn;
		GObject* zoomOut;
		GObject* zoomReset;
		GObject* reloadFiles;
		GObject* viewDownStairs;
		GObject* buttonEye;
		GObject* toggleButtonScript;
		GObject* buttonAutoBorder;
		GObject* buttonMagicWand;
		def::DrawingToolMode drawingMode;
		def::DrawingToolMode drawingModePrevious;



		static void static_cb_signalGtkToggleButtonScript(GtkToggleButton* togglebutton, gpointer user_data);
		static void static_cb_signalGtkToggleButtonEye(GtkToggleButton* togglebutton, gpointer user_data);
		static void static_cb_signalGtkToggleButtonZoomIn(GtkButton* togglebutton, gpointer user_data);
		static void static_cb_signalGtkToggleButtonZoomOut(GtkButton* togglebutton, gpointer user_data);
		static void static_cb_signalGtkToggleButtonZoomReset(GtkButton* togglebutton, gpointer user_data);
		static void static_cb_signalGtkButtonViewDownStairs(GtkButton* togglebutton, gpointer user_data);
		static void static_cb_signalGtkButtonReloadfiles(GtkButton* togglebutton, gpointer user_data);
		static void static_cb_signalGtkButtonAutoBorder(GtkToggleButton* togglebutton, gpointer user_data);
		static void static_cb_signalGtkButtonMagicWand(GtkToggleButton* togglebutton, gpointer user_data);
		
		/**
		 *  @brief Auxiliary function to avoid static members.
		 */
		static void static_cb_signalGtkToggleButtonBrush(GtkToggleButton* togglebutton, gpointer user_data);

		/**
		 *  @brief Auxiliary function to avoid static members.
		 */
		static void static_cb_signalGtkToggleButtonEraser(GtkToggleButton* togglebutton, gpointer user_data);

		/**
		 *  @brief This methods checks if all toggle button are false.
		 */
		bool areAllToogleFalse();

	public:
		/**
		 * constructor.
		 */
		DrawingToolUI();
		
		/**
		 * @brief This methods gets the drawing mode.
		 */
		def::DrawingToolMode getDrawingMode() const;

		/**
		 * @brief This methods set the drawing mode.
		 */
		void setPreviousDrawingMode(def::DrawingToolMode);

		/**
		 * @brief This methods gets the previous drawing mode.
		 */
		def::DrawingToolMode gePrevioustDrawingMode() const;

		/**
		 * @brief This methods set the previousdrawing mode.
		 */
		void setDrawingMode(def::DrawingToolMode);

		/**
		 * @brief This method occurs when the toggle button brush is triggered.
		 */
		void cb_signalGtkToggleButtonBrush(GtkToggleButton* togglebutton, gpointer user_data);

		/**
		 * @brief This method occurs when the toggle button erase is triggered.
		 */
		void cb_signalGtkToggleButtonEraser(GtkToggleButton* togglebutton, gpointer user_data);

		void cb_signalGtkToggleButtonScript(GtkToggleButton* togglebutton, gpointer user_data);
		void cb_signalGtkToggleButtonEye(GtkToggleButton* togglebutton, gpointer user_data);
		void cb_signalGtkToggleButtonZoomIn(GtkButton* togglebutton, gpointer user_data);
		void cb_signalGtkToggleButtonZoomOut(GtkButton* togglebutton, gpointer user_data);
		void cb_signalGtkToggleButtonZoomReset(GtkButton* togglebutton, gpointer user_data);
		void cb_signalGtkButtonViewDownStairs(GtkButton* button, gpointer user_data);
		void cb_signalGtkButtonReloadfiles(GtkButton* togglebutton, gpointer user_data);
		void cb_signalGtkButtonAutoBorder(GtkToggleButton* togglebutton, gpointer user_data);
		void cb_signalGtkButtonMagicWand(GtkToggleButton* togglebutton, gpointer user_data);

		bool is_brush_enable();
		bool is_eraser_enable();
		bool is_script_enable();
		bool is_autoborder_enable();
		bool is_magicwand_enable();

		void disableAllToggleButtonsMinus(GtkToggleButton* togglebutton, enum def::DrawingToolMode mode);
		GtkToggleButton * getToggleButton(enum def::DrawingToolMode mode);
	};

}

