#pragma once
#include <gtk/gtk.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Map.h"
#include <epoxy/gl.h>
#include "GLRect.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

/*!
    GLScene
    ========

    This class represents an encapsulation of the opengl API so that it is possible to draw;
*/

class GLScene
{
public:
    GLScene(scene::Map *, float cameraX, float cameraY, float cameraZ); // used map width, height //
    /**
    *  @brief This method render all objects into the scene.
    */
    static gboolean static_render(GtkGLArea* area, GdkGLContext* context, gpointer user_data);
    gboolean render(GtkGLArea* area, GdkGLContext* context);
    /**
    *  @brief This method is used to initialize OpenGL state.
    */
    static gboolean static_realize(GtkGLArea* area, gpointer user_data);
    gboolean realize(GtkGLArea* area);
    /**
    *  @brief This method is called when user resize opengl screen.
    */
    static void static_resize(GtkGLArea* area, gint width, gint height, gpointer user_data);
    void resize(GtkGLArea* area, gint width, gint height);
   
    /**
    *  @brief This method returns opengl screen width.
    */
    int getWidth();
    /**
    *  @brief This method returns opengl screen height.
    */
    int getHeigth();
    /**
    *  @brief This method returns opengl Widget Area.
    */
    GtkWidget* getGLArea();
    
    /////////////////////////////////////
    /// GRAPHICAL FUNCTIONS           ///
    /// ///////////////////////////// ///
    /**
    *  @brief This method adds a square into opengl square vectos.
    *         The internal map Things are drawn through texture over those squares.
    */
    void addQuad(int line, int col, float floor, float size, glm::vec4 color);

    /**
    *  @brief This method updates selection quad.
    */
    void updateSelectionQuad(glm::vec2 A, glm::vec2 B, glm::vec4 colorRect, glm::vec4 colorBorder, float zCoord);
    
    void disableQuadSelection();
    void enableQuadSelection();
    void disableQuadShadow();
    void enableQuadShadow();
    
    /**
    *  @brief This method returns a quad from quad std::vector.
    */    
    GLRect& getQuad(int index);

    /**
    *  @brief This method changes scale factor value.
    */
    void setScaleFactor(float scale);

    /**
    *  @brief This method returns camera center.
    */
    glm::vec3 getCameraCenter();
   
    /**
    *  @brief This method returns screen coords to world coords.
    */
    glm::vec2 screen_to_world(glm::vec2 screen, int w = 0, int h = 0);

    /**
    *  @brief This method returns finds two points (leftTop,rightBot) From two points.
    */
    void get_LeftTop_rightbot(glm::vec2 A, glm::vec2 B, glm::vec2& leftTop, glm::vec2& rightBot);
    
    /**
    *  @brief This method sets camera center.
    */
    void setCamera(glm::vec2 centerXY, float centerZ = 0.0);
    
    /**
    *  @brief This method translates camera.
    */
    void translateCamera(glm::vec2);
    
    /**
    *  @brief Zoom functions.
    */
    void resetZoom();
    void zoomIn();
    void zoomOut();
    void scale(glm::vec3 scale);
    
    void removeLightCylindergMapUI(int index);
    void addLightCylindergMapUI(int index);

    GLRect _shadowSquare;

    /**
     * @brief This methods return the GRID map size multiplied by scale factor (zoom).
     */
    float getRealGridSize();    

private:
    scene::Map* map;
    GtkWidget* _gtkGLArea;
    GtkWidget* gridImg;
    bool quad_selection_on;
    bool quad_shadow_on;
    
    glm::mat4 _projection; /**< projection matrix  */
    glm::mat4 _camera; /**< camera matrix  */
    glm::vec3 _camera_center; /**< center of camera  */
    glm::mat4 _mvp;  /**< mvp matrix  */
    float scaleFactor;  /**< scale factor used with zoom  */

    GLuint gl_program;
    GLuint gl_vao[10];
    GLuint vertex_buffer[10];
    GLuint tex[2];
    
    std::vector<GLRect> _quads; /**< square (quads) of map  */
    GLRect _selection[2]; /**< square used during selection interior and border */
    int width; /**< screen width  */
    int height; /**< screen height  */

    glm::vec3 _cameraJson;

};