#pragma once
#include <gtk/gtk.h>
#include "MapResources.h"
#include "StuffBookUI.h"
    

namespace ui {
	/*!
		Auxiliary graphics module
		=========================

		This class represents all auxiliary graphical interface.
	 */
	class AuxUI 
	{
	private:
		static GObject* gtkEntryThingSearch;   
		static GObject* gtkStatusBar;   

	public:
		
		/**  
		 *	constructor
		 */
		AuxUI();  

		/**
		 *  @brief This method get GtkStatusbar.
		 */
		static GtkStatusbar* getStatusBar() { return GTK_STATUSBAR(gtkStatusBar); }
		
	    /**
		 *  @brief This method searches for a thing by name. 
		 *  If find then select it into user interface GtkTreeView.
		 *  @param name name to be searched.		
		 *  @return true if found and false otherwise.
		 */
		static bool searchThingByName(std::string name);

	    /**
		 *  @brief This method cleans text input when focused.
		 *  @param widget that will recieve the signal.		
		 *  @param data extra information if needed. 		
		 */
		static void cb_onFocusInEvent(GtkWidget* widget, gpointer data);

	    /**
		 *  @brief This method add text input "..." when not focused.
		 *  @param widget that will recieve the signal.	
		 *  @param data extra information if needed.
		 */
		static void cb_onFocusOutEvent(GtkWidget* widget, gpointer data);		

	    /**
		 *  @brief This method call searchThingByName when activated.
		 *  @param widget that will recieve the signal.	
		 *  @param data extra information if needed.
		 *  @see searchThingByName
		 */		
		static void cb_onActive(GtkWidget* widget, gpointer data);
	};

}
