#pragma once
#ifdef TME_DEBUG
#include <gtk/gtk.h>
#include <vector>
#include <string>
#include <stack>

/*!
 Class DebugTextureAtlas
 =======================

 This class was created to test the data structure ImgPack: insertion, deletion and memory reallocation algorithms.
*/
class DebugTextureAtlas
{
public:
	/**
	 * constructor.
	 * 
	 * @param pixlBuff of TextureAtlas for easy viewing of tests.
	 */
	DebugTextureAtlas(GdkPixbuf* pixlBuff);

	/**
	 * rendering function.
	 */
	static GtkWidget*  p_window; /**< new window where will be drawn */
	static GtkWidget* drawingArea; /**< gtk widget drawing area*/
	static cairo_surface_t* surface;  /**< surface of drawing surface*/
	static GObject* gtkMapFrame;  /**< container to assist */
	static int imgNamesInt; /**< used to create unique imgs 1, 2, 3, ... */
	static std::vector<std::string> imgNames; /**< vector to store imgs created */
	static std::stack<std::string> operations; /**< stack operations type */

	/**
	 * @brief This function adds a new random img (32x32,32x64 or 64x64) into texture atlas pixel buffer.
	 * It is used to test INSERTION algorithm.
	 */
	static gboolean addImgs();
	/**
	 * @brief This function removes a random from texture atlas pixel buffer.
	 * It is used to test REMOVAL algorithm.
	 */
	static gboolean remImgs();

	/**
	 * @brief This function adds a new type of operation randomly on top of stack.
	 */
	static gboolean pop_operations(gpointer data);

	/**
	 * @brief This function removes the last operation add in the stack.
	 */
	static gboolean push_operations(gpointer data);

	/**
	 * @brief This function is used to render the TextureAtlas into the new debug window. 
	 */	
	static gboolean cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data);
};
#endif // TME_DEBUG



