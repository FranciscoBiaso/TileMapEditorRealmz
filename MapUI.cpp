#include "MapUI.h"
#include "MapResources.h"
#include "AuxUI.h"
#include "DrawingFunctions.h"
#include "DrawingToolUI.h"
#include "CtrlMap.h"
#include "Thing.h"
#include "SceneScripts.h"
#include "ScriptUI.h"
#ifdef _WIN32
#include <Windows.h>
#endif

extern ui::ScriptUI* gScriptUI;
extern ui::StuffBookUI* gStuffBook;
extern data::MapResources* gResources;
extern ui::AuxUI* gAuxUI;
extern ui::DrawingToolUI* gDrawingToolUI;
extern ctrl::CtrlMap* ctrlMap;
extern Scripts::ContainerSceneScript* gSceneScripts;

namespace GtkUserInterface { extern GtkBuilder* builder; }

GdkPixbuf* ui::MapUI::_pixelbuf_full_Grid = NULL;

ui::MapUI::MapUI(std::string name, int width, int height, int levels) : Map(name,width,height, levels)
{
    for (int i = 0; i < levels; i++)
        isFloorTransparency.push_back(false);
        
    //worldFloor = levels - 1;
    gtkMapViewPort = gtk_builder_get_object(GtkUserInterface::builder, "gtkMapViewPort");
    gtkMapFrame = gtk_builder_get_object(GtkUserInterface::builder, "gtkMapFrame");
    scrolledwindowMapUI = gtk_builder_get_object(GtkUserInterface::builder, "scrolledwindowMapUI");
    _gtk_label_mouse_coords =  gtk_builder_get_object(GtkUserInterface::builder, "GtkLabelMouseCoords");
    
    // load map from json file //
    glm::vec3 cameraJson;
    loadMapInfoFromJson(cameraJson);
    worldFloor = cameraJson.z;

    _glScene = new GLScene((Map*)this, cameraJson.x, cameraJson.y, cameraJson.z);

    this->setGlScene(_glScene);
     //_glScene->setCamera(_cameraJsonVec2, worldFloor);

    loadOpenGLMap();

    gtk_widget_set_can_focus(_glScene->getGLArea(), true);

    gtk_widget_add_events(_glScene->getGLArea(), GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_KEY_PRESS_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_KEY_RELEASE_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_SCROLL_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_LEAVE_NOTIFY_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_ENTER_NOTIFY_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_ENTER_NOTIFY_MASK);
    gtk_widget_add_events(_glScene->getGLArea(), GDK_SCROLL_MASK);



    //gtk_widget_set_size_request(GTK_WIDGET(_glScene->getGLArea()), 100,100);
    gtk_container_add(GTK_CONTAINER(gtkMapViewPort), _glScene->getGLArea());

    
    //g_signal_connect(G_OBJECT(_glScene->getGLArea()), "draw", G_CALLBACK(static_cb_draw_callback), this);
    g_signal_connect(G_OBJECT(_glScene->getGLArea()), "button-press-event", G_CALLBACK(static_cb_clickNotify), this);
    g_signal_connect(G_OBJECT(_glScene->getGLArea()), "button-release-event", G_CALLBACK(static_cb_clickNotify), this);
    g_signal_connect(G_OBJECT(_glScene->getGLArea()), "key-press-event", G_CALLBACK(static_cb_clickNotify), this);
    g_signal_connect(G_OBJECT(_glScene->getGLArea()), "key-release-event", G_CALLBACK(static_cb_clickNotify), this);
    g_signal_connect(G_OBJECT(_glScene->getGLArea()), "scroll-event", G_CALLBACK(static_cb_scroll), this);
    
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

// removed, now the render function is opengl //
gboolean ui::MapUI::cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    return FALSE;
}

void ui::MapUI::static_cb_draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data) 
{
    reinterpret_cast<MapUI*>(data)->cb_draw_callback(widget, cr, data);
}

gboolean ui::MapUI::static_cb_scroll(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    return reinterpret_cast<MapUI*>(user_data)->cb_scroll(widget, event, user_data);
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

    _mouse_coord = glm::vec2(e->x, e->y);
    _mouse_to_world = _glScene->screen_to_world(glm::vec2(_mouse_coord));

    bool mousePositionHasChanged = _mouse_to_world != _mouse_to_world_previous;

    // mouse position normalized //;
    glm::vec2 mouse_coord_nomalized = glm::vec2(std::floor(e->x / REALMZ_GRID_SIZE), std::floor(e->y/ REALMZ_GRID_SIZE))* (float)REALMZ_GRID_SIZE;
    glm::vec2 mouse_coord_nomalized_by_grid = glm::vec2(std::floor(e->x / REALMZ_GRID_SIZE), std::floor(e->y / REALMZ_GRID_SIZE));

    // mouse in world coords formated by 32 grid size//
    // float gridBorderSize = 1;
    _glScene->_shadowSquare.updatePosition(glm::vec3(_glScene->screen_to_world(glm::vec2(_mouse_coord),(getWidth()-1) * REALMZ_GRID_SIZE,(getHeight()-1) * REALMZ_GRID_SIZE), (float)worldFloor - 0.01), REALMZ_GRID_SIZE);
    glm::vec4 shadowSquareColor = glm::vec4(0.9, 0.9, 0.0, 0.25);

    _glScene->_shadowSquare.setColor(shadowSquareColor);
    _glScene->_shadowSquare.reset_textcoord(-1);

    if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_ERASE ||
        gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_BRUSH || 
        gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_SCRIPT)
    {
        _mousePosition_select_to = _glScene->screen_to_world(glm::vec2(_mouse_coord), getWidth() * REALMZ_GRID_SIZE, getHeight() * REALMZ_GRID_SIZE);

        glm::vec2 offset(0,0);
        _GLScene->updateSelectionQuad(_mousePosition_select_from, _mousePosition_select_to + offset, glm::vec4(0.2, 0.3, 0.9,0.45), glm::vec4(0.2, 1.0, 0.7,0.1), (float)worldFloor - 2 * 0.1f / 10.0f);
    }
    
    glm::vec2 world_coords = _glScene->screen_to_world(_mouse_coord, (getWidth() - 1) * REALMZ_GRID_SIZE, (getHeight() - 1) * REALMZ_GRID_SIZE);

    // show floor //
    gtk_label_set_text(GTK_LABEL(_gtk_label_mouse_coords),
            std::string("(" + std::to_string((int)world_coords.x / REALMZ_GRID_SIZE) + ", " +
            std::to_string(-1 * (int)world_coords.y / REALMZ_GRID_SIZE) + ", " +
            std::to_string(worldFloor-(levels-1)) + ")").c_str());

    if (ctrlModes == DRAWING_PEN_SELECTED && mousePositionHasChanged) // we only add new item if mouse square changes //
    {
        addThingSpecial(world_coords);
        ctrlMap->add_last_operation(ctrl::eManipulator::OPERATION);
    }

    if (ctrlModes == DRAWING_ERASER_SELECTED && mousePositionHasChanged) // we only add new item if mouse square changes //
    {
        delThingMapUI(world_coords,std::abs(worldFloor));
    }

    if (ctrlModes == DRAWING_EYE_SELECTED && mousePositionHasChanged) // we only add new item if mouse square changes //
    {
        upateCylinderLight(world_coords, std::abs(worldFloor));
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

        _glScene->setCamera(_camera_on_hit_space + direction * glm::vec2(-1.0,1.0), worldFloor);        
    }
    
    //if(mousePositionHasChanged)
        forceRedraw();
    
    _mouse_to_world_previous = _mouse_to_world;
    return FALSE;
}


gboolean ui::MapUI::cb_scroll(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    if (event->scroll.direction == GDK_SCROLL_UP && isLeftKeyPressed)
    {
        glm::vec3 center = _GLScene->getCameraCenter();
        worldFloor -= 1;
        if (worldFloor <= 0)
            worldFloor = 0;
        
        _GLScene->setCamera(glm::vec2(center.x, center.y), worldFloor);
    }
    else if (event->scroll.direction == GDK_SCROLL_DOWN && isLeftKeyPressed)
    {
        glm::vec3 center = _GLScene->getCameraCenter();
        worldFloor += 1;

        if (worldFloor >= levels - 1)
            worldFloor = levels - 1;
        _GLScene->setCamera(glm::vec2(center.x, center.y), worldFloor);
    }

    // print world floor //
    glm::vec2 world_coords = _glScene->screen_to_world(_mouse_coord, (getWidth() - 1) * REALMZ_GRID_SIZE, (getHeight() - 1) * REALMZ_GRID_SIZE);
    gtk_label_set_text(GTK_LABEL(_gtk_label_mouse_coords),
        std::string("(" + std::to_string((int)world_coords.x / REALMZ_GRID_SIZE) + ", " +
            std::to_string(-1 * (int)world_coords.y / REALMZ_GRID_SIZE) + ", " +
            std::to_string(worldFloor - (levels-1)) + ")").c_str());
    forceRedraw();
    return FALSE;
}

gboolean ui::MapUI::cb_clickNotify(GtkWidget* widget, GdkEvent* event, gpointer user_data)
{
    int floor = std::abs(worldFloor);
    guint button;
    gdk_event_get_button(event, &button);
    if (event->type == GDK_BUTTON_PRESS && button == GDK_BUTTON_PRIMARY)
    {
        glm::vec2 world_coords = _glScene->screen_to_world(_mouse_coord, (getWidth() - 1) * REALMZ_GRID_SIZE, (getHeight() - 1) * REALMZ_GRID_SIZE);
        // BRUSH //
        if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::DRAWING_BRUSH)
        {
            // ctrl add operation to the stack //
            if (thingIsSelected)
            {
                // add thing with auto border if possible //
                addThingSpecial(world_coords);
            }
            
            ctrlModes = DRAWING_PEN_SELECTED;        
        }
        // ERASE //
        else if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::DRAWING_ERASE)
        {
            delThingMapUI(world_coords, std::abs(worldFloor));
            ctrlModes = DRAWING_ERASER_SELECTED;        
        }
        // EYE PENCIL //
        else if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::DRAWING_EYE)
        {         
            upateCylinderLight(world_coords, floor);
            ctrlModes = DRAWING_EYE_SELECTED;
        }
        // NOTHING //
        else if(gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::DRAWING_NONE)
        {
            gAuxUI->printMsg("First selects a drawing tool!");
        
        }
        else if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_ERASE || 
                 gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_BRUSH || 
                 gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_SCRIPT)
         {
          
          glm::vec2 leftTop, rightBot;
          _GLScene->get_LeftTop_rightbot(_mousePosition_select_from, _mousePosition_select_to, leftTop, rightBot);
            


          if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_ERASE ||
              gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_BRUSH)
          {
              leftTop.y *= -1;
              rightBot.y *= -1;
              if (leftTop.x < 0)
                  leftTop.x = 0;
              if (rightBot.x < 0)
                  rightBot.x = 0;
                  leftTop /= REALMZ_GRID_SIZE;
                  rightBot /= REALMZ_GRID_SIZE;
                  // shift to left-top -> right-bottom //
                  for (int line = leftTop.y; line < rightBot.y; line++)
                      for (int col = leftTop.x; col < rightBot.x; col++)
                      {
                          if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_ERASE)
                              delThingMapUI(line, col, std::abs(worldFloor));
                          if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_BRUSH)
                              addThingMapUI(line, col, std::abs(worldFloor));
                      }
          }
          if (gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_SCRIPT)
          {
              gSceneScripts->addScript(Scripts::SceneScript(leftTop, rightBot, floor));
              gScriptUI->updateTree();
          }
          
        }
    }
    else if (event->type == GDK_BUTTON_PRESS && button == GDK_BUTTON_SECONDARY)
    {
        glm::vec2 world_coords = _glScene->screen_to_world(_mouse_coord, (getWidth() - 1) * REALMZ_GRID_SIZE, (getHeight() - 1) * REALMZ_GRID_SIZE);
        scene::Cylinder& cylinder = at(world_coords.y / REALMZ_GRID_SIZE * -1, world_coords.x / REALMZ_GRID_SIZE, floor);
        // only add light over things //
        if (isCylinderGrid(cylinder)) return TRUE;    
        gStuffBook->selectThing(cylinder.getTopItem());
        gStuffBook->updateTree();
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
        _GLScene->disableQuadShadow();
        
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
          _GLScene->zoomIn();
          
      }
      else if (event->key.keyval == GDK_KEY_minus || event->key.keyval == GDK_KEY_endash)
      {
          _GLScene->zoomOut();
          
      }
      else if (event->key.keyval == GDK_KEY_S || event->key.keyval == GDK_KEY_s)
      {
          _GLScene->setScaleFactor(1.0);
          _GLScene->setCamera(glm::vec2(0,0));
          
      }
      else if (event->key.keyval == GDK_KEY_Control_L)
      {
          isLeftKeyPressed = true;
          
      }

      // SHIFT + SCRIPT BUTTON //

      if (event->key.keyval == GDK_KEY_Shift_L && gDrawingToolUI->getDrawingMode() != def::DrawingToolMode::SELECTING_SCRIPT && 
          gDrawingToolUI->is_script_enable())
      {
          _GLScene->enableQuadSelection();
          gDrawingToolUI->setPreviousDrawingMode(gDrawingToolUI->getDrawingMode());
          gDrawingToolUI->setDrawingMode(def::DrawingToolMode::SELECTING_SCRIPT);
          _mousePosition_select_from = _GLScene->screen_to_world(_mouse_coord, (getWidth() - 1) * REALMZ_GRID_SIZE, (getHeight() - 1) * REALMZ_GRID_SIZE);
          _mousePosition_select_to = _mousePosition_select_from;
          _canDrawSelectionSquare = true;
          _GLScene->disableQuadShadow();
          
      }
      else if (event->key.keyval == GDK_KEY_Shift_L && gDrawingToolUI->getDrawingMode() != def::DrawingToolMode::SELECTING_BRUSH &&
          gDrawingToolUI->is_brush_enable())
      {
          _GLScene->enableQuadSelection();
          gDrawingToolUI->setPreviousDrawingMode(gDrawingToolUI->getDrawingMode());
          gDrawingToolUI->setDrawingMode(def::DrawingToolMode::SELECTING_BRUSH);
          _mousePosition_select_from = _GLScene->screen_to_world(_mouse_coord, (getWidth() - 1) * REALMZ_GRID_SIZE, (getHeight() - 1) * REALMZ_GRID_SIZE);
          _mousePosition_select_to = _mousePosition_select_from;
          _canDrawSelectionSquare = true;
          _GLScene->disableQuadShadow();
      }
      else if (event->key.keyval == GDK_KEY_Shift_L && gDrawingToolUI->getDrawingMode() != def::DrawingToolMode::SELECTING_ERASE &&
          gDrawingToolUI->is_eraser_enable())
      {
          _GLScene->enableQuadSelection();
          gDrawingToolUI->setPreviousDrawingMode(gDrawingToolUI->getDrawingMode());
          gDrawingToolUI->setDrawingMode(def::DrawingToolMode::SELECTING_ERASE);
          _mousePosition_select_from = _GLScene->screen_to_world(_mouse_coord, (getWidth() - 1) * REALMZ_GRID_SIZE, (getHeight() - 1) * REALMZ_GRID_SIZE);
          _mousePosition_select_to = _mousePosition_select_from;
          _canDrawSelectionSquare = true;
          _GLScene->disableQuadShadow();
      }
      /*
      if(isLeftKeyPressed && (event->key.keyval == GDK_KEY_s || event->key.keyval == GDK_KEY_S))
      {
        gAuxUI->printMsg("Saving map data!");
        gResources->getImgPack().getTextureAtlas()->saveAtlasAsImg();
        gResources->getImgPack().getTextureAtlas()->saveAtlasInfoAsJson();
        gResources->getImgPack().saveImgPackAsJsonFile();
        gResources->saveStuffBook();
        this->saveMap();
      }
      */
    }
    else if (event->type == GDK_KEY_RELEASE)
    {
      if (event->key.keyval == GDK_KEY_space)
      {
        ctrlModes = ctrlModesPrevious;  
        show_shadow_square();  
        _GLScene->enableQuadShadow();
        
      }

      if (event->key.keyval == GDK_KEY_Shift_L && gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_ERASE)
      {
          gDrawingToolUI->setDrawingMode(gDrawingToolUI->gePrevioustDrawingMode());
          _GLScene->disableQuadSelection();
          _mousePosition_select_to = _mousePosition_select_from;          
          _canDrawSelectionSquare = false;
          _GLScene->enableQuadShadow();
          
      }

      if (event->key.keyval == GDK_KEY_Shift_L && gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_BRUSH)
      {
          gDrawingToolUI->setDrawingMode(gDrawingToolUI->gePrevioustDrawingMode()); 
          _GLScene->disableQuadSelection();
          _mousePosition_select_to = _mousePosition_select_from;
          _canDrawSelectionSquare = false;
          _GLScene->enableQuadShadow();
          
      }

      // script //
      if (event->key.keyval == GDK_KEY_Shift_L && gDrawingToolUI->getDrawingMode() == def::DrawingToolMode::SELECTING_SCRIPT)
      {
          gDrawingToolUI->setDrawingMode(gDrawingToolUI->gePrevioustDrawingMode());
          _GLScene->disableQuadSelection();
          _mousePosition_select_to = _mousePosition_select_from;
          _canDrawSelectionSquare = false;
          _GLScene->enableQuadShadow();
      }

      else if (event->key.keyval == GDK_KEY_Control_L)
      {
          isLeftKeyPressed = false;
         
      }
    }
    selectCursor();
    forceRedraw();
    return FALSE;
}

void ui::MapUI::setDrawThingObj(data::Thing thing)
{
    drawObj = thing;
    thingIsSelected = true;
}

// map thingName -> array 8 thingsNames

data::Thing ui::MapUI::addThingMapUI(int line, int col, int level)
{
    data::Thing ret;
    if (thingIsSelected)
    {
        ret = addThing(drawObj, line, col, level);
    }
    else
    {
        gAuxUI->printMsg("You need To select a Thing before drawn!");
    }
    return ret;
}

data::Thing ui::MapUI::addThingMapUI(glm::vec2 world_coords, int level)
{
    // format data 
    world_coords.x /= REALMZ_GRID_SIZE;
    world_coords.y /= REALMZ_GRID_SIZE * -1;

    data::Thing ret;
    // mouse x is col, y is row //
    if (thingIsSelected)
    {
        ret = addThing(drawObj, (int)world_coords.y, (int)world_coords.x, level);
    }
    else
    {
        gAuxUI->printMsg("You need To select a Thing before drawn!");
    }
    return ret;
}

data::Thing ui::MapUI::addThingMapUI(glm::vec2 world_coords, int level, data::Thing thing)
{
    // format data 
    world_coords.x /= REALMZ_GRID_SIZE;
    world_coords.y /= REALMZ_GRID_SIZE * -1;

    data::Thing ret;
    // mouse x is col, y is row //
    if (thingIsSelected)
    {
        ret = addThing(thing, (int)world_coords.y, (int)world_coords.x, level);
    }
    else
    {
        gAuxUI->printMsg("You need To select a Thing before drawn!");
    }
    return ret;
}

data::Thing ui::MapUI::addThingMapUI(math::Vec2<int> worl_coords, float level)
{
    data::Thing ret;
    // mouse x is col, y is row //
    if (thingIsSelected)
    {
        ret = addThing(drawObj, worl_coords.getY(), worl_coords.getX(), level);
        gAuxUI->printMsg("Thing " + drawObj.getName() + " added as ["+ drawObj.getType() + "]!");
        
    }
    else
    {
        gAuxUI->printMsg("You need To select a Thing before drawn!");
    }
    return ret;
}

void ui::MapUI::delThingMapUI(int line, int col, int level)
{
    this->cleansCylinder(line, col, level);
    int index = (line * getWidth() + col) + level * getWidth() * getHeight();
    _glScene->getQuad(index).setTextCoord(0, 0, 32, 32);

    //_GLScene->addLightCylindergMapUI(index);

    this->structure[level][width * line + col].setLight(true);

    if (isFloorTransparency[worldFloor])
        _glScene->getQuad(index).setColor(glm::vec4(1, 1, 0.75, 0.3));
    else
        _glScene->getQuad(index).setColor(glm::vec4(1, 1, 1, 0.5));
}

void ui::MapUI::delThingMapUI(glm::vec2 world_coords, int level)
{
    // format data 
    world_coords.x /= REALMZ_GRID_SIZE;
    world_coords.y /= REALMZ_GRID_SIZE * -1.0;
    int index = (world_coords.y * getWidth() + world_coords.x) + level * getWidth() * getHeight();

    this->cleansCylinder(world_coords.y, world_coords.x, level);
    _glScene->getQuad(index).setTextCoord(0,0,32,32);

    this->structure[level][width * world_coords.y + world_coords.x].setLight(true);

    //_GLScene->addLightCylindergMapUI(index);    
    if(isFloorTransparency[worldFloor])
        _glScene->getQuad(index).setColor(glm::vec4(1, 1, 0.75,0.3));
    else
        _glScene->getQuad(index).setColor(glm::vec4(1, 1, 1, 0.5));

}

void ui::MapUI::delThingMapUI(std::string thing_name, math::Vec3<int> thing_position)
{
    this->removeThing(thing_name, thing_position.getY(), thing_position.getX(), thing_position.getZ());
}

void ui::MapUI::delThingMapUI(data::Thing* ptr)
{
    math::Vec3 coords = ptr->getCylinder()->getCoords();
    this->removeThing(ptr->getName(), coords.getY(), coords.getX(), coords.getZ());
    int index = (coords.getY() * getWidth() + coords.getX()) + std::abs(worldFloor) * getWidth() * getHeight();
    _glScene->getQuad(index).setTextCoord(0, 0, 32, 32);
    _glScene->getQuad(index).setColor(glm::vec4(1, 1, 1, 0));
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
    float alpha = 0.5f; // THIS VALUE IS IMPORTANTE, CHECK GLSL SHADER //
    float adjust = 0.1; // space to draw selection quad //
    for (int level = 0; level < levels; level++)
    {
        for (int line = 0; line < getHeight(); line++)
        {
            for (int col = 0; col < getWidth(); col++)
            {   
                
                _glScene->addQuad(-line, col, (level - adjust) * 1.0f, REALMZ_GRID_SIZE - gridBorderSize, glm::vec4(1,1,1, alpha));
                _GLScene->getQuad((line * getWidth() + col) + level * getWidth() * getHeight()).setTextCoord(0, 0, 32, 32);

            }
        }
    }
}

void ui::MapUI::loadMapInfoFromJson(glm::vec3 & cameraVec3Json)
{
    std::ifstream ifs("resources//map_info.json");// file to read //
    Json::CharReaderBuilder rbuilder;	// reader //
    std::string errs; // to check errors //
    Json::Value jsonObj;
    data::Thing thing;
    Json::parseFromStream(rbuilder, ifs, &jsonObj, &errs); // parser //   
    if (!jsonObj.isNull()) // loading img pack //
    {
        cameraVec3Json = glm::vec3(jsonObj["0_camera_x"].asInt(), jsonObj["0_camera_y"].asInt(), jsonObj["0_camera_z"].asInt());
    }
}

void ui::MapUI::loadRandomTilesFromJson()
{
    std::ifstream ifs("resources//random_tiles.json");// file to read //
    Json::CharReaderBuilder rbuilder;	// reader //
    std::string errs; // to check errors //
    Json::Value jsonObj;
    Json::parseFromStream(rbuilder, ifs, &jsonObj, &errs); // parser //   
    if (!jsonObj.isNull())
    {
        Json::Value mainObj = jsonObj["mainObj"];

        for (int i = 0; i < mainObj.size(); i++)
        {

            Json::Value thingName = mainObj[i]["thing_name"];
            Json::Value radom_tiles = mainObj[i]["randomTiles"];

            std::vector<RandomTile> v;
            for (int j = 0; j < radom_tiles.size(); j++)
            {
                RandomTile randomTile;
                randomTile.name = radom_tiles[j]["name"].asString();
                randomTile.percentage = radom_tiles[j]["percentage"].asDouble();
                v.push_back(randomTile);
            }

            _randomTiles[thingName.asString()] = v;
        }
    }
    else
    {
        gAuxUI->printMsg("Failed to load autoborder file");
    }
}

void ui::MapUI::loadAutoBorderFromJson()
{
    std::ifstream ifs("resources//auto_border.json");// file to read //
    Json::CharReaderBuilder rbuilder;	// reader //
    std::string errs; // to check errors //
    Json::Value jsonObj;
    Json::parseFromStream(rbuilder, ifs, &jsonObj, &errs); // parser //   
    if (!jsonObj.isNull())
    {
        Json::Value thingsAutoBorders = jsonObj["thingsAutoBorders"];

        for (int i = 0; i < thingsAutoBorders.size(); i++)
        {
            std::vector<std::string> bordersStr;

            Json::Value borders = thingsAutoBorders[i]["border_left"];
            if (!borders.isNull())  bordersStr.push_back(borders.asString()); else bordersStr.push_back("");
            borders = thingsAutoBorders[i]["border_right"];
            if (!borders.isNull())   bordersStr.push_back(borders.asString()); else bordersStr.push_back("");
            borders = thingsAutoBorders[i]["border_top"];
            if (!borders.isNull())   bordersStr.push_back(borders.asString()); else bordersStr.push_back("");
            borders = thingsAutoBorders[i]["border_bot"];
            if (!borders.isNull())   bordersStr.push_back(borders.asString()); else bordersStr.push_back("");
            borders = thingsAutoBorders[i]["border_left-top"];
            if (!borders.isNull())   bordersStr.push_back(borders.asString()); else bordersStr.push_back("");
            borders = thingsAutoBorders[i]["border_right-top"];
            if (!borders.isNull())   bordersStr.push_back(borders.asString()); else bordersStr.push_back("");
            borders = thingsAutoBorders[i]["border_left-bot"];
            if (!borders.isNull())   bordersStr.push_back(borders.asString()); else bordersStr.push_back("");
            borders = thingsAutoBorders[i]["border_right-bot"];
            if (!borders.isNull())   bordersStr.push_back(borders.asString()); else bordersStr.push_back("");

            Json::Value thingName = thingsAutoBorders[i]["thing_name"];
            _autoBorders[thingName.asString()] = bordersStr;
        }
    }
    else
    {
        gAuxUI->printMsg("Failed to load autoborder file");
    }
}

void ui::MapUI::loadMapFromJson()
{
    loadInternalMapFromJson();
}

void ui::MapUI::saveMapInfoAsJson()
{
    Json::Value root;
    Json::Value jsonArrayCylinders(Json::arrayValue);
    root["0_camera_x"] = _glScene->getCameraCenter().x;
    root["0_camera_y"] = _glScene->getCameraCenter().y;
    root["0_camera_z"] = _glScene->getCameraCenter().z;

    Json::StreamWriterBuilder builder;

    builder["commentStyle"] = "None";
    builder["indentation"] = "   ";
    std::unique_ptr<Json::StreamWriter> writer(
        builder.newStreamWriter());

    // Make a new JSON document for the configuration. Preserve original comments.
    std::ofstream ofs("resources//map_info.json", std::ofstream::out);// file to read //
    writer->write(root, &ofs);

    ofs.close();
}

bool ui::MapUI::hasAutoBorder(std::string thingName, std::vector<std::string> & borders)
{    
    for (auto it = _autoBorders.begin(); it != _autoBorders.end(); it++) // iterate through stuffbook //
    {
        if (it->first == thingName)
        {
            borders = it->second;
            return true;
        }
    }
    return false;
}

bool ui::MapUI::hasRandomTile(std::string thingName, std::vector<RandomTile>& rTiles)
{
    for (auto it = _randomTiles.begin(); it != _randomTiles.end(); it++) // iterate through stuffbook //
    {
        if (it->first == thingName)
        {
            rTiles = it->second;
            return true;
        }
    }
    return false;
}

void ui::MapUI::addThingSpecial(glm::vec2 coord)
{   
    std::vector <std::string> borders;
    std::vector<int> listToNotIterate;
    std::map<int, int> listToFill;
    bool hasTile[8];
    data::Thing tmp;
    if (gDrawingToolUI->is_autoborder_enable() && hasAutoBorder(drawObj.getName(), borders))
    {   
        // 0 left left 
        for(int i=0;i<borders.size();i++)
            if (borders[i] != "")
            {
                glm::vec2 mapCoords = coord + getOffset(i);
                                
                try
                {
                    scene::Cylinder& cylinder = at(mapCoords.y / REALMZ_GRID_SIZE * -1, mapCoords.x / REALMZ_GRID_SIZE, std::abs(worldFloor));
                    if (cylinder.hasThingByName(drawObj.getName()))
                        hasTile[i] = true;
                    else
                        hasTile[i] = false;

                    if (i == 0 && cylinder.hasThingByName(drawObj.getName())) // LEFT
                    {
                        listToNotIterate.push_back(4);
                        listToNotIterate.push_back(6);
                    }

                    if (i == 1 && cylinder.hasThingByName(drawObj.getName())) // RIGHT
                    {
                        listToNotIterate.push_back(5);
                        listToNotIterate.push_back(7);
                    }

                    if (i == 2 && cylinder.hasThingByName(drawObj.getName())) // TOP
                    {
                        listToNotIterate.push_back(4);
                        listToNotIterate.push_back(5);
                    }

                    if (i == 3 && cylinder.hasThingByName(drawObj.getName())) // BOT    
                    {
                        listToNotIterate.push_back(6);
                        listToNotIterate.push_back(7);
                    }
                }
                catch (def::ReturnMsg & msg)
                {
                    if (msg == def::ReturnMsg::ARRAY_OUT_OF_RANGE)
                    {
                        continue;
                    }
                }

                
            }

        
        for (int i = 0; i < borders.size(); i++)
            if (borders[i] != "")
            {
                glm::vec2 mapCoords = coord + getOffset(i);
                
                try
                {
                    scene::Cylinder& cylinder = at(mapCoords.y / REALMZ_GRID_SIZE * -1, mapCoords.x / REALMZ_GRID_SIZE, std::abs(worldFloor));
                    auto result = std::find(listToNotIterate.begin(), listToNotIterate.end(), i);
                    if (result != std::end(listToNotIterate))
                    {
                        continue;
                    }

                    if (gStuffBook->getThingByName(borders[i], tmp) && isIntoMap(mapCoords) && !cylinder.hasThingByName(drawObj.getName()))
                    {
                        tmp.setStuffBookRefName(borders[i]);
                        ctrlMap->add_ctrlz(ctrl::sOperation(ctrl::eOperation::ADD_THING, addThingMapUI(mapCoords, std::abs(worldFloor), tmp)));
                        ctrlMap->add_last_operation(ctrl::eManipulator::OPERATION);
                    }
                }
                catch (def::ReturnMsg& msg)
                {
                    if (msg == def::ReturnMsg::ARRAY_OUT_OF_RANGE)
                    {
                        continue;
                    }
                }
            }


        // SINGLES //
        if (hasTile[def::Combinations::LEFT_BOT]) 
        {
            listToFill[def::Combinations::LEFT] = def::Combinations::RIGHT_BOT;
            listToFill[def::Combinations::BOT] = def::Combinations::LEFT_TOP;
        }
        if (hasTile[def::Combinations::LEFT_TOP])
        {
            listToFill[def::Combinations::LEFT] = def::Combinations::RIGHT_TOP;
            listToFill[def::Combinations::TOP] = def::Combinations::LEFT_BOT;
        }
        if (hasTile[def::Combinations::RIGHT_TOP])
        {
            listToFill[def::Combinations::RIGHT] = def::Combinations::LEFT_TOP;
            listToFill[def::Combinations::TOP] = def::Combinations::RIGHT_BOT;
        }
        if (hasTile[def::Combinations::RIGHT_BOT])
        {
            listToFill[def::Combinations::RIGHT] = def::Combinations::LEFT_BOT;
            listToFill[def::Combinations::BOT] = def::Combinations::RIGHT_TOP;
        }
        // check diagonal combinations DOUBLES//
        if(hasTile[def::Combinations::LEFT_TOP] && hasTile[def::Combinations::TOP])
        { 
            listToFill[def::Combinations::LEFT] = def::Combinations::RIGHT_TOP;
        }
        if (hasTile[def::Combinations::TOP] && hasTile[def::Combinations::RIGHT_TOP])
        {
            listToFill[def::Combinations::RIGHT] = def::Combinations::LEFT_TOP;
        }
        // 2
        if (hasTile[def::Combinations::LEFT_BOT] && hasTile[def::Combinations::BOT])
        {
            listToFill[def::Combinations::LEFT] = def::Combinations::RIGHT_BOT;
        }
        if (hasTile[def::Combinations::BOT] && hasTile[def::Combinations::RIGHT_BOT])
        {
            listToFill[def::Combinations::RIGHT] = def::Combinations::LEFT_BOT;
        }
        // 3
        if (hasTile[def::Combinations::LEFT] && hasTile[def::Combinations::LEFT_TOP])
        {
            listToFill[def::Combinations::TOP] = def::Combinations::LEFT_BOT;
        }
        if (hasTile[def::Combinations::RIGHT] && hasTile[def::Combinations::RIGHT_TOP])
        {
            listToFill[def::Combinations::TOP] = def::Combinations::RIGHT_BOT;
        }
        // 4
        if (hasTile[def::Combinations::RIGHT] && hasTile[def::Combinations::RIGHT_BOT])
        {
            listToFill[def::Combinations::BOT] = def::Combinations::RIGHT_TOP;
        }
        if (hasTile[def::Combinations::LEFT] && hasTile[def::Combinations::LEFT_BOT])
        {
            listToFill[def::Combinations::BOT] = def::Combinations::LEFT_TOP;
        }

        for (auto it = listToFill.begin(); it != listToFill.end(); it++)
        {
            glm::vec2 mapCoords = coord + getOffset(it->first);
            
            try
            {
                scene::Cylinder& cylinder = at(mapCoords.y / REALMZ_GRID_SIZE * -1, mapCoords.x / REALMZ_GRID_SIZE, std::abs(worldFloor));
                if (!cylinder.hasThingByName(drawObj.getName()))
                {
                    gStuffBook->getThingByName(borders[it->second], tmp);                    
                    tmp.setStuffBookRefName(borders[it->second]);

                    ctrlMap->add_ctrlz(ctrl::sOperation(ctrl::eOperation::ADD_THING, addThingMapUI(mapCoords, std::abs(worldFloor), tmp)));
                    ctrlMap->add_last_operation(ctrl::eManipulator::OPERATION);
                }
            }
            catch (def::ReturnMsg& msg)
            {
                if (msg == def::ReturnMsg::ARRAY_OUT_OF_RANGE)
                {
                    continue;
                }
            }
           
            
        }

    }

    std::vector <RandomTile> randomTiles;
    // add only once //
    std::string thingToAdd;
    thingToAdd = drawObj.getName();

    if (gDrawingToolUI->is_magicwand_enable() && hasRandomTile(drawObj.getName(), randomTiles) && !randomTiles.empty())
    {
        int random = std::rand() % randomTiles.size();
        if (randomTiles[random].percentage >= std::rand() % 101)
        {
            thingToAdd = randomTiles[random].name;
        }
    }

    gStuffBook->getThingByName(thingToAdd, tmp);
    tmp.setStuffBookRefName(thingToAdd);

    ctrlMap->add_ctrlz(ctrl::sOperation(ctrl::eOperation::ADD_THING, addThingMapUI(coord, std::abs(worldFloor), tmp)));
    ctrlMap->add_last_operation(ctrl::eManipulator::OPERATION);
    
}


glm::vec2 ui::MapUI::getOffset(int index)
{   
    if (index == 0) return glm::vec2(-REALMZ_GRID_SIZE, 0); // left 
    if (index == 1) return glm::vec2(REALMZ_GRID_SIZE, 0); // right 
    if (index == 2) return glm::vec2(0, REALMZ_GRID_SIZE); // top
    if (index == 3) return glm::vec2(0, -REALMZ_GRID_SIZE); // bot
    if (index == 4) return glm::vec2(-REALMZ_GRID_SIZE, REALMZ_GRID_SIZE); // left top
    if (index == 5) return glm::vec2(REALMZ_GRID_SIZE, REALMZ_GRID_SIZE); // r top
    if (index == 6) return glm::vec2(-REALMZ_GRID_SIZE, -REALMZ_GRID_SIZE); // l b
    if (index == 7) return glm::vec2(REALMZ_GRID_SIZE, -REALMZ_GRID_SIZE); // r b
    return  glm::vec2(0, 0);
}

bool ui::MapUI::isIntoMap(glm::vec2 coords)
{
    coords.x /= REALMZ_GRID_SIZE;
    coords.y /= REALMZ_GRID_SIZE * -1;
    
    if (coords.x < 0 || coords.x >= getWidth() ||
        coords.y < 0 || coords.y >= getHeight())
        return false;
    return true;
}

void  ui::MapUI::cleanAutoBorders()
{
    _autoBorders.clear();
}

void ui::MapUI::cleanRandomTiles()
{
    _randomTiles.clear();
}

void ui::MapUI::setCanSeeDownStairs(bool value)
{
    if (!isFloorTransparency[worldFloor])
    {
        isFloorTransparency[worldFloor] = true;
        // alpha = 0.4 add transparency, see frag glsl //
        updateGlSceneColorFloor(std::abs(worldFloor), glm::vec4(1, 1, 0.75, 0.3));
    }
    else
    {
        isFloorTransparency[worldFloor] = false;
        // alpha = 0.5 become opaque see frag glsl //
        updateGlSceneColorFloor(std::abs(worldFloor), glm::vec4(1, 1, 1, 0.5));
    }
}

void ui::MapUI::updateGlSceneColorFloor(int floor, glm::vec4 color)
{
    for (int line = 0; line < getHeight(); line++)
    {
        for (int col = 0; col < getWidth(); col++)
        {
            int index = (line * getWidth() + col) + floor * getWidth() * getHeight();
            GLRect& quad = _glScene->getQuad(index);
            if(std::abs(quad.T1.colorA.a - 0.5) < 0.01) // this is a grid
                quad.setColor(color);
            if(std::abs(quad.T1.colorA.a - 0.3) < 0.01) // this is a grid
                quad.setColor(color);
        }
    }
}

void ui::MapUI::upateCylinderLight(glm::vec2 world_coords, int floor)
{
    try
    {
        scene::Cylinder & cylinder = at(world_coords.y / REALMZ_GRID_SIZE * -1, world_coords.x / REALMZ_GRID_SIZE, floor);
        // only add light over things //
        if (isCylinderGrid(cylinder)) return;

        world_coords.x /= REALMZ_GRID_SIZE;
        world_coords.y /= REALMZ_GRID_SIZE * -1.0;
        int index = (world_coords.y * getWidth() + world_coords.x) + floor * getWidth() * getHeight();

        if (cylinder.hasLight())
        {
            // update thing attribute cylinder //
            cylinder.setLight(false);
            // update graphics color //
            _GLScene->removeLightCylindergMapUI(index);
        }
        else
        {
            cylinder.setLight(true);
            _GLScene->addLightCylindergMapUI(index);
        }
    }
    catch (def::ReturnMsg& msg)
    {
        if (msg == def::ReturnMsg::ARRAY_OUT_OF_RANGE)
        {}
    }
}

bool ui::MapUI::isCylinderGrid(scene::Cylinder & cylinder)
{
    return cylinder.isEmpty();
}

int ui::MapUI::getLevels()
{
    return this->levels;
}