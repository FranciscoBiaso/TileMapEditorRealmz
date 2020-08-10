#pragma once
#include <gtk/gtk.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Map.h"
#include <epoxy/gl.h>
//class scene::Map;

typedef struct Triangle {
    Triangle() {}
    Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec2 A_text = glm::vec2(0, 0), glm::vec2 B_text = glm::vec2(0, 0), glm::vec2 C_text = glm::vec2(0, 0))
    {
        this->A = A;
        this->B = B;
        this->C = C;
        setTextCoord(A_text, B_text, C_text);
    }
    void setColor(glm::vec4 a, glm::vec4 b, glm::vec4 c);
    void setTextCoord(glm::vec2 a, glm::vec2 b, glm::vec2 c);
    glm::vec3 A;
    glm::vec4 colorA;
    glm::vec2 A_text_coord;
    glm::vec3 B;
    glm::vec4 colorB;
    glm::vec2 B_text_coord;
    glm::vec3 C;
    glm::vec4 colorC;
    glm::vec2 C_text_coord;
};

typedef struct Quad {
    /*
        A-------------D
        |             |
        |             |
        |             |
        |             |
        |             |
        B-------------C
    */
    Quad() {}
    Quad(glm::vec3 leftTop, float side)
    {
        // Opengl Coords //
        /*
                   U
          (0,0) ---------> 1
            |
           V|
            |
            v
            1
        */
        updatePosition(leftTop, side);
    }
    void updatePosition(glm::vec3 leftTop, float topSide, float leftSide = 0);
    void setTextCoord(int line, int col, int w = -1, int h = -1);
    void setTextCoord(glm::vec2);
    // a is left top //
    void setColor(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d);
    void setColor(glm::vec4 color);
    void reset_textcoord(float val = -1);
    void reset_color();
    // first triangle // A C B //
    Triangle T1;
    // seconds triangle // C D B //
    Triangle T2;
}Quad;

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

class GLScence
{
public:
    GLScence(scene::Map *);
    static gboolean static_render(GtkGLArea* area, GdkGLContext* context, gpointer user_data);
    static gboolean static_realize(GtkGLArea* area, gpointer user_data);
    static void static_resize(GtkGLArea* area, gint width, gint height, gpointer user_data);
    
    gboolean render(GtkGLArea* area, GdkGLContext* context);
    gboolean realize(GtkGLArea* area);
    void resize(GtkGLArea* area, gint width, gint height);

    int getWidth();
    int getHeigth();
    GtkWidget* getGLArea();
    void addQuad(int line, int col, int layer, float size, glm::vec4 color);
    void setCamera(glm::vec2);
    void translateCamera(glm::vec2);
    void updateSelectionQuad(glm::vec2 A, glm::vec2 B, glm::vec4 colorRect, glm::vec4 colorBorder);
    void disableQuadSelection();
    void enableQuadSelection();
    void disableQuadShadow();
    void enableQuadShadow();
    Quad& getQuad(int line, int col);
    void setScaleFactor(float scale);
    glm::vec3 getCameraCenter();
    Quad& getQuad(int index);
    Quad& getQuad(glm::vec2 coords);
    glm::vec2 screen_to_world(glm::vec2 screen);
    glm::vec2 screen_to_world_by_grid_size(glm::vec2 screen);
    void get_LeftTop_rightbot(glm::vec2 A, glm::vec2 B, glm::vec2& leftTop, glm::vec2& rightBot);

    void zoomIn();
    void zoomOut();

    Quad _shadowSquare;    
private:
    GtkWidget* gridImg;
    bool quad_selection_on;
    bool quad_shadow_on;
    scene::Map* map;
    GtkWidget* _gtkGLArea;
    glm::mat4 _projection;
    glm::mat4 _camera;
    glm::mat4 _mvp;
    GLuint gl_vao[10];
    GLuint vertex_buffer[10];
    GLuint gl_program;
    GLuint tex[2];
    guchar* pixels;
    glm::vec3 _camera_center;

    std::vector<Quad> _quads;
    Quad _selection[2]; // quad used during selection //
    int width;
    int height;
    glm::vec2 scale;
    float scaleFactor;
};