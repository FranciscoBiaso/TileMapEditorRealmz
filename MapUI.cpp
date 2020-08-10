#include "MapUI.h"
#include "MapResources.h"
#include "AuxUI.h"
#include "DrawingFunctions.h"
#include "DrawingToolUI.h"
#include "CtrlMap.h"
#include "Thing.h"
#ifdef _WIN32
#include <Windows.h>
#endif

extern data::MapResources* gResources;
extern ui::AuxUI* gAuxUI;
extern ui::DrawingToolUI* gDrawingToolUI;
extern ctrl::CtrlMap* ctrlMap;

namespace GtkUserInterface { extern GtkBuilder* builder; }

GdkPixbuf* ui::MapUI::_pixelbuf_full_Grid = NULL;

ui::MapUI::MapUI(std::string name, int width, int height) : Map(name,width,height)
{

    gtkMapViewPort = gtk_builder_get_object(GtkUserInterface::builder, "gtkMapViewPort");
    gtkMapFrame = gtk_builder_get_object(GtkUserInterface::builder, "gtkMapFrame");
    scrolledwindowMapUI = gtk_builder_get_object(GtkUserInterface::builder, "scrolledwindowMapUI");
    _gtk_label_mouse_coords =  gtk_builder_get_object(GtkUserInterface::builder, "GtkLabelMouseCoords");
    
    _glScene = new GLScence((Map*)this);
    this->setGlScene(_glScene);
    loadOpenGLMap();

    gtk_widget_set_can_focus(_glScene->getGLArea(), true);

    gtk_widget_add_events(_glScene->getGLArea(), GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_KEY_PRESS_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_KEY_RELEASE_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_LEAVE_NOTIFY_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_ENTER_NOTIFY_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_ENTER_NOTIFY_MASK);


    //gtk_widget_set_size_request(GTK_WIDGET(_glScene->getGLArea()), 100,100);
    gtk_container_add(GTK_CONTAINER(gtkMapViewPort), _glScene->getGLArea());

    
    //g_signal_connect(G_OBJECT(_glScene->getGLArea()), "draw", G_CALLBACK(static_cb_draw_callback), this);
    g_signal_connect(G_OBJECT(_glScene->getGLArea()), "button-press-event", G_CALLBACK(static_cb_clickNotify), this);
    g_signal_connect(G_OBJECT(_glScene->getGLArea()), "button-release-event", G_CALLBACK(static_cb_clickNotify), this);
    g_signal_connect(G_OBJECT(_glScene->getGLArea()), "key-press-event", G_CALLBACK(static_cb_clickNotify), this);
    g_signal_connect(G_OBJECT(_glScene->getGLArea()), "key-release-event", G_CALLBACK(static_cb_clickNotify), this);
    
    g_signal_connect(G_OBJECT(_glScene->getGLArea()), "motion-notify-event", G_CALLBACK(static_cb_MotionNotify), this);
    g_signal_connect(G_OBJECT(_glScene->getGLArea()), "enter-notify-event", G_CALLBACK(static_cb_onEnter), this);
    g_signal_connect(G_OBJECT(_glScene->getGLArea()), "leave-notify-event", G_CALLBACK(static_cb_onLeave), this);
    
    g_signal_connect(gtkMapViewPort, "size-allocate", G_CALLBACK(static_my_getsize), this);    

    thingIsSelected = false;
    ctrlModes = DRAWING_EMPTY;

    cursorPixelbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE);

    gridColor.setXYZ(0.95f, 0.95f, 0.1f);
    canDrawMouseShadowSquare = false;
    _map_layer = 0;

    if (_pixelbuf_unity_grid == nullptr)
	{
		GError* err = NULL;
		_pixelbuf_unity_grid = gdk_pixbuf_new_from_file("ui_imgs//grid_unity.png", &err);
        _surface_grid = gdk_cairo_surface_create_from_pixbuf(_pixelbuf_unity_grid, 0, NULL);
	}
    _grid_enable = true;

    camera_at(math::Vec2(0, 0));
    _camera_move_speed = REALMZ_GRID_SIZE;
    camera_set_delta(REALMZ_GRID_SIZE);

    enable_draw_map_borders();
    show_shadow_square();
    _canDrawSelectionSquare = false;
}

void ui::MapUI::static_my_getsize(GtkWidget* widget, GtkAllocation* allocation, void* data)
{
    return reinterpret_cast<MapUI*>(data)->map_resize(widget, allocation, allocation);
}

math::Vec2<int> ui::MapUI::screen_coords_to_world_coords(math::Vec2<int> screen)
{
    /*
    int world_coords_x = (- / 2 + _glScene->getCameraCenter().x) + screen.getX();
    int world_coords_y = (-viewHeight / 2 + _glScene->getCameraCenter().y) + screen.getY();
    if (world_coords_x < 0)
        world_coords_x = 0;
    if(world_coords_x > getWidth())
        world_coords_x = getWidth();
    if (world_coords_y < 0)
        world_coords_y = 0;
    if(world_coords_y > getHeight())
        world_coords_y = getHeight();
         return math::Vec2<int>(world_coords_x, world_coords_y);
}
*/
    return math::Vec2<int>(0, 0);
}
   

// removed, now the render function is opengl //
gboolean ui::MapUI::cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    return FALSE;
}

void ui::MapUI::static_cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data) 
{
    reinterpret_cast<MapUI*>(data)->cb_draw_callback(widget, cr, data);
}

gboolean ui::MapUI::static_cb_clickNotify(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    return reinterpret_cast<MapUI*>(user_data)->cb_clickNotify(widget, event, user_data);
}

gboolean ui::MapUI::static_cb_MotionNotify(GtkWidget* widget, GdkEventMotion* event, gpointer user_data)
{
    return reinterpret_cast<MapUI*>(user_data)->cb_MotionNotify(widget, event, user_data);
}

gboolean ui::MapUI::static_cb_onEnter(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    return reinterpret_cast<MapUI*>(user_data)->cb_onEnter(widget, event, user_data);
}

gboolean ui::MapUI::static_cb_onLeave(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    return reinterpret_cast<MapUI*>(user_data)->cb_onLeave(widget, event, user_data);
}

gboolean ui::MapUI::cb_MotionNotify(GtkWidget* widget, GdkEventMotion* e, gpointer user_data)
{
    if (mouseStartPositionToMoveMapView.getX() == 0 &&
        mouseStartPositionToMoveMapView.getY() == 0)
    {
        mouseStartPositionToMoveMapView.setXY(e->x, e->y);
    }

    mousePosition_by_32.setX((int)(e->x / REALMZ_GRID_SIZE));
    mousePosition_by_32.setY((int)(e->y / REALMZ_GRID_SIZE));
    mousePosition_windows_coord.setXY(e->x, e->y);

    //_glScene->_shadowSquare.updatePosition(glm::vec3(-64, 64, 0), REALMZ_GRID_SIZE);
    // update shadow square position //
    math::Vec2<int> screen_coords = screen_coords_to_world_coords(mousePosition_windows_coord);    


    _mouse_coord = glm::vec2(e->x, e->y);
    _mouse_to_world = _glScene->screen_to_world(glm::vec2(_mouse_coord));

    bool mousePositionHasChanged = _mouse_to_world != _mouse_to_world_previous;

    // mouse position normalized //;
    glm::vec2 mouse_coord_nomalized = glm::vec2(std::floor(e->x / REALMZ_GRID_SIZE), std::floor(e->y/ REALMZ_GRID_SIZE))* (float)REALMZ_GRID_SIZE;
    glm::vec2 mouse_coord_nomalized_by_grid = glm::vec2(std::floor(e->x / REALMZ_GRID_SIZE), std::floor(e->y / REALMZ_GRID_SIZE));

    // mouse in world coords formated by 32 grid size//
    // float gridBorderSize = 1;
    _glScene->_shadowSquare.updatePosition(glm::vec3(_glScene->screen_to_world(glm::vec2(_mouse_coord)),0), REALMZ_GRID_SIZE);
    glm::vec4 shadowSquareColor = glm::vec4(1, 0.8, 0.6, 0.7);

    _glScene->_shadowSquare.setColor(shadowSquareColor);
    _glScene->_shadowSquare.reset_textcoord(-1);

    if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_ERASE ||
        gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_BRUSH)
    {
        _mousePosition_select_to = _glScene->screen_to_world(glm::vec2(_mouse_coord));
        glm::vec2 offset(0,0);
        //if (_mousePosition_select_from.x < _mousePosition_select_to.x && _mousePosition_select_from.y < _mousePosition_select_to.y)
           // offset = glm::vec2(-REALMZ_GRID_SIZE, -REALMZ_GRID_SIZE);
        //if(_mousePosition_select_from.x != _mousePosition_select_to.x && _mousePosition_select_from.y != _mousePosition_select_to.y)
            _glScence->updateSelectionQuad(_mousePosition_select_from, _mousePosition_select_to + offset, glm::vec4(0.2, 0.3, 0.9,0.4), glm::vec4(0.2, 1.0, 0.7,0.5));
    }

    math::Vec2<int> world_coords = screen_coords_to_world_coords(mousePosition_by_32);

    gtk_label_set_text(GTK_LABEL(_gtk_label_mouse_coords), mouse_coords_to_word_position_to_string(screen_coords_to_world_coords(mousePosition_by_32)).c_str());

    if (ctrlModes == DRAWING_PEN_SELECTED && mousePositionHasChanged) // we only add new item if mouse square changes //
    {
        glm::vec2 world_coords = _glScene->screen_to_world(_mouse_coord);

        // ctrl add operation to the stack //
        ctrlMap->add_ctrlz(ctrl::sOperation(ctrl::eOperation::ADD_THING, addThingMapUI(world_coords)));
        ctrlMap->add_last_operation(ctrl::eManipulator::OPERATION);
    }

    if (ctrlModes == DRAWING_ERASER_SELECTED && mousePositionHasChanged  ) // we only add new item if mouse square changes //
    {
        glm::vec2 world_coords = _glScene->screen_to_world(_mouse_coord);
        delThingMapUI(world_coords);
        ctrlMap->add_last_operation(ctrl::eManipulator::OPERATION);
    }

    if (ctrlModes == MOVING_VIEW_OF_MAP)
    {
       // if (mousePosition_by_32 == (mouseStartPositionToMoveMapView * (1.0/REALMZ_GRID_SIZE)))
          //  return TRUE;
        // mouse distance vector //

        mapDetachment.setX(mousePosition_windows_coord.getX() - mouseStartPositionToMoveMapView.getX());
        mapDetachment.setY(mousePosition_windows_coord.getY() - mouseStartPositionToMoveMapView.getY());
        glm::vec2 direction = glm::vec2(mapDetachment.getX(), mapDetachment.getY());
        glm::vec2 source = glm::vec2(mouseStartPositionToMoveMapView.getX(), mouseStartPositionToMoveMapView.getY());

        math::Vec2 v = _camera_position_when_user_press_space + mapDetachment;
        float speed = 1.0f;

        _glScene->setCamera(_camera_on_hit_space + direction * glm::vec2(-1.0,1.0));
        
        //camera_block();


    }
    
    //if(mousePositionHasChanged)
        forceRedraw();

    mousePositionPrevious = mousePosition_by_32;

    _mouse_to_world_previous = _mouse_to_world;
    return TRUE;
}

gboolean ui::MapUI::cb_clickNotify(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    if (event->type == GDK_BUTTON_PRESS)
    {
      if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::DRAWING_BRUSH)
      {
        glm::vec2 world_coords = _glScene->screen_to_world(_mouse_coord);

        // ctrl add operation to the stack //
        ctrlMap->add_ctrlz(ctrl::sOperation(ctrl::eOperation::ADD_THING, addThingMapUI(world_coords)));
        ctrlMap->add_last_operation(ctrl::eManipulator::OPERATION);

        mousePositionPrevious = mousePosition_by_32;
        ctrlModes = DRAWING_PEN_SELECTED;
      }
      else if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::DRAWING_ERASE)
      {
        glm::vec2 world_coords = _glScene->screen_to_world(_mouse_coord);

        // ctrl add operation to the stack //
        //ctrlMap->add_ctrlz(ctrl::sOperation(ctrl::eOperation::REMOVE_THING, delThingMapUI(world_coords)));
        delThingMapUI(world_coords);
        ctrlModes = DRAWING_ERASER_SELECTED;
      }
      else if(gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::DRAWING_NONE)
      {
        gAuxUI->printMsg("First selects a drawing tool!");
      }
      else if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_ERASE || 
               gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_BRUSH)
      {
          
          glm::vec2 leftTop, rightBot;
          _glScence->get_LeftTop_rightbot(_mousePosition_select_from, _mousePosition_select_to, leftTop, rightBot);
            
          leftTop /= REALMZ_GRID_SIZE;
          rightBot /= REALMZ_GRID_SIZE;

          leftTop.y *= -1;
          rightBot.y *= -1;
          if (leftTop.x < 0)
              leftTop.x = 0;
          if (rightBot.x < 0)
              rightBot.x = 0;
          // shift to left-top -> right-bottom //
          for (int line = leftTop.y; line < rightBot.y; line++)
              for (int col = leftTop.x; col < rightBot.x; col++)
              {
                  if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_ERASE)
                    delThingMapUI(line,col);
                  if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_BRUSH)
                    addThingMapUI(line, col);
              }
      }
    }
    else if (event->type == GDK_BUTTON_RELEASE)
    {
        ctrlModes = DRAWING_EMPTY;
    }

    if (event->type == GDK_KEY_PRESS)
    {
      if(event->key.keyval == GDK_KEY_space && ctrlModes != MOVING_VIEW_OF_MAP)
      {
        ctrlModesPrevious = ctrlModes;
        ctrlModes = MOVING_VIEW_OF_MAP;
        mouseStartPositionToMoveMapView = math::Vec2<int>(0,0);
        _camera_position_when_user_press_space = camera_get_position();

        _camera_on_hit_space = _glScene->getCameraCenter();
        hide_shadow_square();
        _glScence->disableQuadShadow();
      }

      // CTRL + Z //
      if ((event->key.keyval == GDK_KEY_z || event->key.keyval == GDK_KEY_Z) &&
          event->key.state == GDK_CONTROL_MASK)
      {          
          if (!ctrlMap->empty())
          {
              ctrlMap->add_last_operation(ctrl::eManipulator::CTRL_Z);
              ctrl::sOperation operation = ctrlMap->rem_ctrlz();
              do_reverse_operation(operation);
              ctrlMap->add_ctrly(operation.swap_operation()); // add this op into the another stack //
          }
      }

      // CTRL + Y //
      if ((event->key.keyval == GDK_KEY_y || event->key.keyval == GDK_KEY_Y) &&
          event->key.state == GDK_CONTROL_MASK)
      {
          if (!ctrlMap->inv_empty())
          {
              ctrlMap->add_last_operation(ctrl::eManipulator::CTRL_Y);
              ctrl::sOperation operation = ctrlMap->rem_ctrly();
              do_reverse_operation(operation);
              ctrlMap->add_ctrlz(operation.swap_operation()); // add this op into the another stack //
          }
      }
      // MOVING CAMERA //
      if (event->key.keyval == GDK_KEY_w || event->key.keyval == GDK_KEY_W)
      {
          camera_move_top();
      }
      else if (event->key.keyval == GDK_KEY_d || event->key.keyval == GDK_KEY_D)
      {
          camera_move_right();
      }
      else if (event->key.keyval == GDK_KEY_s || event->key.keyval == GDK_KEY_S)
      {
          camera_move_bottom();
      }
      else if (event->key.keyval == GDK_KEY_a || event->key.keyval == GDK_KEY_A)
      {
          camera_move_left();
      }

      else if (event->key.keyval == GDK_KEY_plus || event->key.keyval == GDK_KEY_equal)
      {
          _glScence->zoomIn();
      }
      else if (event->key.keyval == GDK_KEY_minus || event->key.keyval == GDK_KEY_endash)
      {
          _glScence->zoomOut();
      }
      else if (event->key.keyval == GDK_KEY_S || event->key.keyval == GDK_KEY_s)
      {
          _glScence->setScaleFactor(1.0);
          _glScence->setCamera(glm::vec2(0,0));
      }



      if (event->key.keyval == GDK_KEY_Shift_L && gDrawingToolUI->getDrawingMode() != def::DrawingToolMode::SELECTING_ERASE && 
          gDrawingToolUI->is_eraser_enable())
      {
          _glScence->enableQuadSelection();
          gDrawingToolUI->setPreviousDrawingMode(gDrawingToolUI->getDrawingMode());
          gDrawingToolUI->setDrawingMode(def::DrawingToolMode::SELECTING_ERASE);
          _mousePosition_select_from = _glScence->screen_to_world(_mouse_coord);
          _mousePosition_select_to = _mousePosition_select_from;
          _canDrawSelectionSquare = true;
          _glScence->disableQuadShadow();
      }
      else if (event->key.keyval == GDK_KEY_Shift_L && gDrawingToolUI->getDrawingMode() != def::DrawingToolMode::SELECTING_BRUSH &&
          gDrawingToolUI->is_brush_enable())
      {
          _glScence->enableQuadSelection();
          gDrawingToolUI->setPreviousDrawingMode(gDrawingToolUI->getDrawingMode());
          gDrawingToolUI->setDrawingMode(def::DrawingToolMode::SELECTING_BRUSH);
          _mousePosition_select_from = _glScence->screen_to_world(_mouse_coord);
          _mousePosition_select_to = _mousePosition_select_from;
          _canDrawSelectionSquare = true;
          _glScence->disableQuadShadow();
      }
    }
    else if (event->type == GDK_KEY_RELEASE)
    {
      if (event->key.keyval == GDK_KEY_space)
      {
        ctrlModes = ctrlModesPrevious;  
        show_shadow_square();  
        _glScence->enableQuadShadow();
      }

      if (event->key.keyval == GDK_KEY_Shift_L && gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_ERASE)
      {
          gDrawingToolUI->setDrawingMode(gDrawingToolUI->gePrevioustDrawingMode());
          _glScence->disableQuadSelection();
          _mousePosition_select_to = _mousePosition_select_from;          
          _canDrawSelectionSquare = false;
          _glScence->enableQuadShadow();
          
      }

      if (event->key.keyval == GDK_KEY_Shift_L && gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_BRUSH)
      {
          gDrawingToolUI->setDrawingMode(gDrawingToolUI->gePrevioustDrawingMode()); 
          _glScence->disableQuadSelection();
          _mousePosition_select_to = _mousePosition_select_from;
          _canDrawSelectionSquare = false;
          _glScence->enableQuadShadow();
          
      }
    }
    selectCursor();
    forceRedraw();
    return TRUE;
}

void ui::MapUI::setDrawThingObj(data::Thing thing)
{
    drawObj = thing;

    thingIsSelected = true;
}

data::Thing ui::MapUI::addThingMapUI(int line, int col)
{
    data::Thing ret;
    if (thingIsSelected)
    {
        ret = addThing(drawObj, line, col, 0);
    }
    else
    {
        gAuxUI->printMsg("You need To select a Thing before drawn!");
    }
    return ret;
}

data::Thing ui::MapUI::addThingMapUI(glm::vec2 world_coords)
{
    // format data 
    world_coords.x /= REALMZ_GRID_SIZE;
    world_coords.y /= REALMZ_GRID_SIZE * -1.0;
    data::Thing ret;
    // mouse x is col, y is row //
    if (thingIsSelected)
    {
        ret = addThing(drawObj, (int)world_coords.y, (int)world_coords.x, 0);                
    }
    else
    {
        gAuxUI->printMsg("You need To select a Thing before drawn!");
    }
    return ret;
}

data::Thing ui::MapUI::addThingMapUI(math::Vec2<int> worl_coords)
{
    data::Thing ret;
    // mouse x is col, y is row //
    if (thingIsSelected)
    {
        ret = addThing(drawObj, worl_coords.getY(), worl_coords.getX(), 0);
        gAuxUI->printMsg("Thing " + drawObj.getName() + " added as ["+ drawObj.getType() + "]!");
        
    }
    else
    {
        gAuxUI->printMsg("You need To select a Thing before drawn!");
    }
    return ret;
}


void ui::MapUI::delThingMapUI(int line, int col)
{
    this->cleansCylinder(line, col, 0);
    _glScene->getQuad(line, col).setTextCoord(0, 0, 32, 32);
    _glScene->getQuad(line, col).setColor(glm::vec4(1, 1, 1, 0));    
}

void ui::MapUI::delThingMapUI(glm::vec2 world_coords)
{
    // format data 
    world_coords.x /= REALMZ_GRID_SIZE;
    world_coords.y /= REALMZ_GRID_SIZE * -1.0;
    this->cleansCylinder(world_coords.y, world_coords.x, 0);
    _glScene->getQuad(world_coords).setTextCoord(0,0,32,32);
    _glScene->getQuad(world_coords).setColor(glm::vec4(1,1,1,0));    
}

void ui::MapUI::delThingMapUI(math::Vec2<int> woord_coords)
{
  this->cleansCylinder(woord_coords.getY(), woord_coords.getX(), 0);  
}

void ui::MapUI::delThingMapUI(std::string thing_name, math::Vec3<int> thing_position)
{
    this->removeThing(thing_name, thing_position.getY(), thing_position.getX(), thing_position.getZ());
}

void ui::MapUI::delThingMapUI(data::Thing* ptr)
{
    math::Vec3 coords = ptr->getCylinder()->getCoords();
    this->removeThing(ptr->getName(), coords.getY(), coords.getX(), coords.getZ());
    _glScene->getQuad(coords.getY(), coords.getX()).setTextCoord(0, 0, 32, 32);
    _glScene->getQuad(coords.getY(), coords.getX()).setColor(glm::vec4(1, 1, 1, 0));
}

void ui::MapUI::selectCursor()
{
    // has priority  //
    switch (ctrlModes)
    {
    case MOVING_VIEW_OF_MAP:
        gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(_glScene->getGLArea())), gdk_cursor_new_for_display(gdk_display_get_default(), GDK_FLEUR));
        return;
    default:
        break;
    }

    switch(gDrawingToolUI->getDrawingMode())
    {
    case def::DrawingToolMode::DRAWING_NONE:
        gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(_glScene->getGLArea())), gdk_cursor_new_for_display(gdk_display_get_default(), GDK_ARROW));
        break;
    case def::DrawingToolMode::DRAWING_BRUSH:
    {
        if (drawObj.getImgObjPtr() != nullptr)
        {
            math::Vec2<int> textureAtlasReference = drawObj.getImgObjPtr()->getRef(0);
            gdk_pixbuf_copy_area(gResources->getImgPack().getTextureAtlas()->getPixelbuf(), textureAtlasReference.getX() * REALMZ_GRID_SIZE, textureAtlasReference.getY() * REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, cursorPixelbuf, 0, 0);

            gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(_glScene->getGLArea())), gdk_cursor_new_from_pixbuf(gdk_display_get_default(), cursorPixelbuf, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2));
        }
        else
            gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(_glScene->getGLArea())), gdk_cursor_new_for_display(gdk_display_get_default(), GDK_PENCIL));
    }
        break;
    case def::DrawingToolMode::DRAWING_ERASE:
        gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(_glScene->getGLArea())), gdk_cursor_new_for_display(gdk_display_get_default(), GDK_SPRAYCAN));
        break;    
    case def::DrawingToolMode::SELECTING_ERASE:
    case  def::DrawingToolMode::SELECTING_BRUSH:
        gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(_glScene->getGLArea())), gdk_cursor_new_for_display(gdk_display_get_default(), GDK_ICON));
        break;
    }
    

}

gboolean ui::MapUI::cb_onEnter(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    // grab focus, we can detect keyboard keys //
    gtk_widget_grab_focus(_glScene->getGLArea());
    // changes the cursor pixelbuf //
    selectCursor();
    // change grid color //
    gridColor.setXYZ(0.95f, 0.95f, 0.1f);
    canDrawMouseShadowSquare = true;
    return TRUE;
}

gboolean ui::MapUI::cb_onLeave(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    // outside mapUI we use the default cursor //
    gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(_glScene->getGLArea())), gdk_cursor_new_for_display(gdk_display_get_default(), GDK_ARROW));
    // reset ctrls //
    ctrlModes = DRAWING_EMPTY;
    // change grid color //
    gridColor.setXYZ(0.5f,0.5f,0.5f);
    canDrawMouseShadowSquare = false;
    forceRedraw();
    return TRUE;
}

void ui::MapUI::deletAllThingsFromTheMap(std::string thingName)
{
  deletAllThings(thingName);
}

void ui::MapUI::forceRedraw()
{
  gtk_widget_queue_draw(GTK_WIDGET(_glScene->getGLArea())); // force redraw map //
}

void ui::MapUI::do_reverse_operation(ctrl::sOperation operation)
{
    switch (operation._operation)
    {
    case ctrl::eOperation::ADD_THING:
    {
        delThingMapUI(&operation._thing);
    }
    break;
    case ctrl::eOperation::REMOVE_THING:
    {
        math::Vec3 position = operation._thing.getCylinder()->getCoords();
        addThing(operation._thing, position.getY(), position.getX(), position.getZ());
    }
    break;
    default:
        break;
    }
}

void ui::MapUI::map_resize(GtkWidget* widget, GtkAllocation* allocation, void* data)
{        
    viewWidth = allocation->width;
    viewHeight = allocation->height;
    /*
    if (_pixelbuf_full_Grid == NULL)
        _pixelbuf_full_Grid = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, viewWidth + REALMZ_GRID_SIZE, viewHeight + REALMZ_GRID_SIZE);
    
    if (_pixelbuf_full_Grid != NULL)
    {
        GError* err = NULL;
        for (int l = 0; l < viewHeight; l+= REALMZ_GRID_SIZE)
            for (int c = 0; c < viewWidth; c+= REALMZ_GRID_SIZE)
                gdk_pixbuf_copy_area(_pixelbuf_unity_grid, 0, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, _pixelbuf_full_Grid, c, l );

        _surface_grid = gdk_cairo_surface_create_from_pixbuf(_pixelbuf_full_Grid, 0, NULL);
        g_object_unref(_pixelbuf_full_Grid);
        _pixelbuf_full_Grid = NULL;
    }
    */
    forceRedraw();
}

void ui::MapUI::camera_at(math::Vec2<int> position)
{
    _camera_position.setXY(position.getX(), position.getY());
}
void ui::MapUI::camera_set_delta(int delta)
{
    _camera_delta = delta;
}
void ui::MapUI::camera_increment_delta(int step)
{
    _camera_delta += step;
}
void ui::MapUI::camera_decrement_delta(int step)
{
    _camera_delta -= step;
}
void ui::MapUI::camera_move_right()
{
    _camera_position.setX(_camera_position.getX() + _camera_delta);
}
void ui::MapUI::camera_move_top()
{
    _camera_position.setY(_camera_position.getY() - _camera_delta);
}
void ui::MapUI::camera_move_bottom()
{
    _camera_position.setY(_camera_position.getY() + _camera_delta);
}
void ui::MapUI::camera_move_left()
{
    _camera_position.setX(_camera_position.getX() - _camera_delta);
}
void ui::MapUI::camera_move(int dx, int dy)
{
    _camera_position.setXY(_camera_position.getX() + dx, _camera_position.getY() + dy);
}

void ui::MapUI::hide_shadow_square()
{
    canDrawMouseShadowSquare = false;
}

void ui::MapUI::show_shadow_square()
{
    canDrawMouseShadowSquare = true;
}

math::Vec2<int> ui::MapUI::camera_get_position()
{
    return _camera_position;
}

void ui::MapUI::camera_block()
{
    if (camera_get_position().getX() < 0)
        _camera_position.setX(0);
    if(camera_get_position().getY() < 0)
        _camera_position.setY(0);

    if (camera_get_position().getX() > getWidth() * REALMZ_GRID_SIZE)
        _camera_position.setX(getWidth() * REALMZ_GRID_SIZE);
    if (camera_get_position().getY() > getHeight() * REALMZ_GRID_SIZE)
        _camera_position.setY(getHeight() * REALMZ_GRID_SIZE);
}


void ui::MapUI::enable_draw_map_borders()
{
    _draw_map_borders = true;
}

void ui::MapUI::disable_draw_map_borders()
{
    _draw_map_borders = false;
}

bool ui::MapUI::can_draw_map_borders()
{
    return _draw_map_borders;
}

void ui::MapUI::draw_map_ui(cairo_t * cr)
{
    int widthTiles = viewWidth / REALMZ_GRID_SIZE + 2;
    int heightTiles = viewHeight / REALMZ_GRID_SIZE + 2;

    math::Vec2<int> position_center_to_draw;
    double f = 1 / (REALMZ_GRID_SIZE * 1.0);
    position_center_to_draw.setXY(camera_get_position().getX() * f, camera_get_position().getY() * f);
    int x_start_position = position_center_to_draw.getX() - widthTiles / 2;
    if (x_start_position < 0)
        x_start_position = 0;    
    int x_end_position = position_center_to_draw.getX() + widthTiles / 2;
    if (x_end_position > getWidth())
        x_end_position = getWidth();

    int y_start_position = position_center_to_draw.getY() - heightTiles / 2;
    if (y_start_position < 0)
        y_start_position = 0;
    int y_end_position = position_center_to_draw.getY() + heightTiles / 2;
    if (y_end_position > getHeight())
        y_end_position = getHeight();


    if (can_draw_map_borders()) // DRAW BORDER LEFT //
    {
        if (x_start_position == 0)
        {
            //for (int line = y_start_position; line < y_end_position; line++)
        }
    }

    scene::Cylinder * cylinder = nullptr;
    for (int line = y_start_position; line < y_end_position; line++)
    {
        for (int col = x_start_position; col < x_end_position; col++)
        {
            cylinder = &this->structure[0][width * line + col];
            if (_grid_enable)
            {
                cairo_set_source_surface(cr, _surface_grid, cylinder->getCoords().getX() * REALMZ_GRID_SIZE, cylinder->getCoords().getY() * REALMZ_GRID_SIZE);
                cairo_paint(cr);
            }
            // draw things //
            cylinder->draw(cr);
        }
    }
}

std::string ui::MapUI::mouse_coords_to_word_position_to_string(math::Vec2<int> screen_coords)
{
    return std::string("(" + std::to_string(screen_coords.getX()) + ", " + std::to_string(screen_coords.getY()) + ")");
}

void ui::MapUI::loadOpenGLMap()
{
    float gridBorderSize = 0;
    glm::vec4 quadColor(0.2, 0.2, 0.7,0.9);
    float alpha = 0.65;
    for (int line = 0; line < getWidth(); line++)
    {
        for (int col = 0; col < getHeight(); col++)
        {
            _glScene->addQuad(-line, col, 0, REALMZ_GRID_SIZE - gridBorderSize, glm::vec4(0,0,0,0));
            _glScene->getQuad(line, col).setTextCoord(0, 0, 32, 32);
            //_glScene->getQuad(line, col).reset_color();
            //_glScene->getQuad(line, col).setColor(glm::vec4(1, 0, 0, 0.25), glm::vec4(0, 1, 0, 0.25), glm::vec4(0, 0, 1, 0.25), glm::vec4(1, 1, 0, 0.85));
        }
    }
    /*
    float quadSize = 4;
    // draw border corners //
    for (int col = 0; col < getHeight(); col++)
    {
        // top //
        _glScene->addQuad(1, col, 0, quadSize, quadColor, quadColor * percentage1, quadColor * percentage2, quadColor * percentage3);
        // bot //
        _glScene->addQuad(-getHeight(), col, 0, quadSize, quadColor, quadColor * percentage1, quadColor * percentage2, quadColor * percentage3);
    }
    for (int line = 0; line < getWidth(); line++)
    {
        // left //
        _glScene->addQuad(-line, -1, 0, quadSize, quadColor, quadColor * percentage1, quadColor * percentage2, quadColor * percentage3);
        // right //
        _glScene->addQuad(-line, getWidth(), 0, quadSize, quadColor, quadColor * percentage1, quadColor * percentage2, quadColor * percentage3);
    }
    */
}