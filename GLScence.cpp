#include "GLScence.h"
#include "Map.h"
#include "MapResources.h"
#include <glm/glm.hpp>
#include "SceneScripts.h"

using namespace glm;
extern data::MapResources* gResources;
extern Scripts::SceneScripts* gSceneScripts;
GdkPixbuf* _pixbuf;

int vertices = 2 * 3 * 3;
int textures = 2 * 3 * 2;
int colors = 2 * 3 * 4;

// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 330 
    in vec3 position;
    in vec4 color;
    in vec2 texcoord;
    out vec4 Color;
    out vec2 Texcoord;
    uniform mat4 _mvp;
    void main()
    {
        Color = color;
        Texcoord = texcoord;
        gl_Position = _mvp * vec4(position, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 330 
    in vec4 Color;
    in vec2 Texcoord;
    out vec4 outColor;
    uniform sampler2D textureAtlas;
    uniform sampler2D gridTexture;
    float eps = 0.001;
    void main()
    {
        if(abs(Color.a - 0.5) < eps)
        {
            outColor = texture(gridTexture, Texcoord) * vec4(Color.xyz, 1.0);
        }
        else if(abs(Color.a - 0.3) < eps)
        {
            outColor = texture(gridTexture, Texcoord) * Color.xyzw;
        }
        else
        {
            outColor = texture(textureAtlas, Texcoord) * Color.xyzw;
        }

        if(abs(Texcoord.x - -1.0) < eps && abs(Texcoord.y - -1.0) < eps)
        {
            outColor = Color;
        }
    }
)glsl";
// outColor = texture(tex, Texcoord);

GLScence::GLScence(scene::Map* map) : map(map)
{
    _gtkGLArea = gtk_gl_area_new();
    g_signal_connect(_gtkGLArea, "render", G_CALLBACK(static_render), this);
    g_signal_connect(_gtkGLArea, "realize", G_CALLBACK(static_realize), this);
    g_signal_connect(_gtkGLArea, "resize", G_CALLBACK(static_resize), this);
    
    _selection[0].reset_textcoord(-1); // use color not texture //
    _selection[1].reset_textcoord(-1); // use color not texture //

    disableQuadSelection();
    enableQuadShadow();
    gridImg = gtk_image_new_from_file("ui_imgs//unity.png");
    scaleFactor = 1.0f;   
}

GLQuad& GLScence::getQuad(int index)
{
    return _quads[index];
}

void GLScence::setScaleFactor(float scale)
{
    scaleFactor = scale;
}

gboolean GLScence::static_render(GtkGLArea* area, GdkGLContext* context, gpointer user_data)
{
    return reinterpret_cast<GLScence*>(user_data)->render(area, context);
}

gboolean GLScence::static_realize(GtkGLArea* area, gpointer user_data)
{
    return reinterpret_cast<GLScence*>(user_data)->realize(area);
}

void GLScence::static_resize(GtkGLArea* area, gint width, gint height, gpointer user_data)
{
    reinterpret_cast<GLScence*>(user_data)->resize(area, width, height);
}

int GLScence::getWidth(){ return width;}
int GLScence::getHeigth(){return height;}

GtkWidget* GLScence::getGLArea()
{
    return _gtkGLArea;
}

gboolean GLScence::render(GtkGLArea* area, GdkGLContext* context)
{
    glEnable(GL_MULTISAMPLE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glUseProgram(gl_program);
    GLint texLoc1 = glGetUniformLocation(gl_program, "textureAtlas");
    GLint texLoc2 = glGetUniformLocation(gl_program, "gridTexture");
    GLint positionAtt = glGetAttribLocation(gl_program, "position");
    GLint colorAtt = glGetAttribLocation(gl_program, "color");
    GLint texAttrib = glGetAttribLocation(gl_program, "texcoord");
    GLint mvpID = glGetUniformLocation(gl_program, "_mvp");

    // MVP //
    glUniformMatrix4fv(mvpID, 1, GL_FALSE, glm::value_ptr(_mvp));
    
    // TEXTURE 0 //
    glActiveTexture(GL_TEXTURE0 );
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gResources->getImgPack().getTextureAtlas()->getAtlasWidth(),
        gResources->getImgPack().getTextureAtlas()->getAtlasHeight(),
        0, GL_RGBA, GL_UNSIGNED_BYTE, gdk_pixbuf_get_pixels(gResources->getImgPack().getTextureAtlas()->getPixelbuf()));
    glUniform1i(texLoc1, 0);// send texture 0 to shader //
    //////////////////////////
    
    // TEXTURE 1 //
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    _pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(gridImg));
    if (_pixbuf != NULL)
    {
        int width = gdk_pixbuf_get_width(_pixbuf);
        int height = gdk_pixbuf_get_height(_pixbuf);


        guchar* p = gdk_pixbuf_get_pixels(_pixbuf);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, gdk_pixbuf_get_pixels(_pixbuf));
        
    }
    glUniform1i(texLoc2, 1);// send texture 1 to shader //
    /////////////////////////

    // HOW WE WILL DRAW ? //
    glEnableVertexAttribArray(positionAtt);    // We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(positionAtt, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), BUFFER_OFFSET(0));      // The starting point of the VBO, for the vertices
    glEnableVertexAttribArray(colorAtt);    // We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(colorAtt, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));      // The starting point of the VBO, for the vertices
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));


    // BIND ARRAW //
    glBindVertexArray(gl_vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, _quads.size() * (vertices + textures + colors) * sizeof(float), &_quads[0], GL_DYNAMIC_DRAW);
    // DRAW //
    if(_quads.size() != 0)
        glDrawArrays(GL_TRIANGLES, 0, _quads.size() * (vertices + textures + colors));
    //////////////////////
    
    // script rects //
    glBindVertexArray(gl_vao[3]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[3]);
    glBufferData(GL_ARRAY_BUFFER, gSceneScripts->getQuads().size() * (vertices + textures + colors) * sizeof(float), &gSceneScripts->getQuads()[0], GL_DYNAMIC_DRAW);
    if (gSceneScripts->getQuads().size() != 0)
        glDrawArrays(GL_TRIANGLES, 0, gSceneScripts->getQuads().size() * (vertices + textures + colors));


    /// /////////////////////////////
    /// shadow QUAD /////////////////
    /// /////////////////////////////
    if (quad_shadow_on)
    {
        glBindVertexArray(gl_vao[1]);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[1]); // vertex_buffer is retrieved from glGenBuffers
        glBufferData(GL_ARRAY_BUFFER, (vertices + textures + colors) * sizeof(float), &_shadowSquare, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, (vertices + textures + colors));
    }

    ////////////////////////////////////
    /// SELECTION QUAD /////////////////
    /// ////////////////////////////////
    if (quad_selection_on)
    {
        glBindVertexArray(gl_vao[2]); // quad selection //
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[2]); // vertex_buffer is retrieved from glGenBuffers
        glBufferData(GL_ARRAY_BUFFER, 2 /*quads*/ * (vertices + textures + colors) * sizeof(float), _selection, GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, 2 * (vertices + textures + colors));
    }

    // DISABLE //
    glBindVertexArray(0);
    glUseProgram(0);

    glFlush();
    
    return TRUE;
}

void GLScence::addQuad(int line, int col, float floor, float size,glm::vec4 color)
{
    GLQuad q(vec3(col * REALMZ_GRID_SIZE, line * REALMZ_GRID_SIZE, floor), size);
    q.setColor(color);
    _quads.push_back(q);
}

gboolean GLScence::realize(GtkGLArea* area)
{
    // ADD DEPTH BUFFER //
    gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(area), TRUE);    
    // MAKE CONTEXT CURRENT //
    gtk_gl_area_make_current(GTK_GL_AREA(area));
    // CHECK ERRORS //
    if (gtk_gl_area_get_error(GTK_GL_AREA(area)) != NULL)
    {
        printf("failed to initialiize buffers\n");
        return false;
    }
    // LOAD SHADERS //
    GLuint frag_shader, vert_shader;
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    vert_shader = glCreateShader(GL_VERTEX_SHADER);    

    glShaderSource(frag_shader, 1, &fragmentSource, NULL);
    glShaderSource(vert_shader, 1, &vertexSource, NULL);

    glCompileShader(frag_shader);
    glCompileShader(vert_shader);

    gl_program = glCreateProgram();
    glAttachShader(gl_program, frag_shader);
    glAttachShader(gl_program, vert_shader);
    glLinkProgram(gl_program);

    glGenVertexArrays(1, &gl_vao[0]);
    glBindVertexArray(gl_vao[0]);
    glGenBuffers(1, &vertex_buffer[0]);   
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[0]); // vertex_buffer is retrieved from glGenBuffers
    glBufferData(GL_ARRAY_BUFFER, _quads.size() * (vertices + textures + colors) * sizeof(float), &_quads[0], GL_DYNAMIC_DRAW);
    GLint positionAtt = glGetAttribLocation(gl_program, "position");
    GLint colorAtt = glGetAttribLocation(gl_program, "color");
    GLint texAttrib = glGetAttribLocation(gl_program, "texcoord");
    glEnableVertexAttribArray(positionAtt);    // We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(positionAtt, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), BUFFER_OFFSET(0));      // The starting point of the VBO, for the vertices
    glEnableVertexAttribArray(colorAtt);    // We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(colorAtt, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));      // The starting point of the VBO, for the vertices
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));

    glGenVertexArrays(1, &gl_vao[1]);
    glBindVertexArray(gl_vao[1]);
    glGenBuffers(1, &vertex_buffer[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[1]); // vertex_buffer is retrieved from glGenBuffers
    glBufferData(GL_ARRAY_BUFFER, (vertices + textures + colors) * sizeof(float), &_shadowSquare, GL_DYNAMIC_DRAW);    
    positionAtt = glGetAttribLocation(gl_program, "position");
    colorAtt = glGetAttribLocation(gl_program, "color");
    texAttrib = glGetAttribLocation(gl_program, "texcoord");
    glEnableVertexAttribArray(positionAtt);    // We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(positionAtt, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), BUFFER_OFFSET(0));      // The starting point of the VBO, for the vertices
    glEnableVertexAttribArray(colorAtt);    // We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(colorAtt, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));      // The starting point of the VBO, for the vertices
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));

    glGenVertexArrays(1, &gl_vao[2]);
    glBindVertexArray(gl_vao[2]);
    glGenBuffers(1, &vertex_buffer[2]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[2]); // vertex_buffer is retrieved from glGenBuffers
    glBufferData(GL_ARRAY_BUFFER, 2 /*quads*/ * (vertices + textures + colors) * sizeof(float), _selection, GL_DYNAMIC_DRAW);
    positionAtt = glGetAttribLocation(gl_program, "position");
    colorAtt = glGetAttribLocation(gl_program, "color");
    texAttrib = glGetAttribLocation(gl_program, "texcoord");
    glEnableVertexAttribArray(positionAtt);    // We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(positionAtt, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), BUFFER_OFFSET(0));      // The starting point of the VBO, for the vertices
    glEnableVertexAttribArray(colorAtt);    // We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(colorAtt, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));      // The starting point of the VBO, for the vertices
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));

    glGenVertexArrays(1, &gl_vao[3]);
    glBindVertexArray(gl_vao[3]);
    glGenBuffers(1, &vertex_buffer[3]);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[3]); // vertex_buffer is retrieved from glGenBuffers
    glBufferData(GL_ARRAY_BUFFER, (vertices + textures + colors) * sizeof(float), &gSceneScripts->getQuads()[0], GL_DYNAMIC_DRAW);
    positionAtt = glGetAttribLocation(gl_program, "position");
    colorAtt = glGetAttribLocation(gl_program, "color");
    texAttrib = glGetAttribLocation(gl_program, "texcoord");
    glEnableVertexAttribArray(positionAtt);    // We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(positionAtt, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), BUFFER_OFFSET(0));      // The starting point of the VBO, for the vertices
    glEnableVertexAttribArray(colorAtt);    // We like submitting vertices on stream 0 for no special reason
    glVertexAttribPointer(colorAtt, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));      // The starting point of the VBO, for the vertices
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));

   
    float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glGenTextures(2, tex);

    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gResources->getImgPack().getTextureAtlas()->getAtlasWidth(),
                                           gResources->getImgPack().getTextureAtlas()->getAtlasHeight(),
            0, GL_RGBA, GL_UNSIGNED_BYTE, gdk_pixbuf_get_pixels(gResources->getImgPack().getTextureAtlas()->getPixelbuf()));
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    _pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(gridImg));
    if (_pixbuf != NULL)
    {
        int width = gdk_pixbuf_get_width(_pixbuf);
        int height = gdk_pixbuf_get_height(_pixbuf);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, gdk_pixbuf_get_pixels(_pixbuf));
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    

    return TRUE;
}

void GLScence::resize(GtkGLArea* area, gint width, gint height)
{
    this->width = width;
    this->height = height;
    const float aspectRatio = ((float)width) / height;
    float xSpan = 1; // Feel free to change this to any xSpan you need.
    float ySpan = 1; // Feel free to change this to any ySpan you need.
    
    if (aspectRatio > 1) {
        // Width > Height, so scale xSpan accordinly.
        xSpan *= aspectRatio;
    }
    else {
        // Height >= Width, so scale ySpan accordingly.
        ySpan = xSpan / aspectRatio;
    }   
 
    _projection = glm::ortho(
        -width / 2 * 1.0f,
        width / 2 * 1.0f,
        -height / 2 * 1.0f,
        height / 2 * 1.0f,
        -0.001f,5.f
    );    
        
    // Use the entire window for rendering.
    glViewport(0, 0, width, height);
    setCamera(glm::vec2(0, 0), 0);
}

void GLScence::setCamera(glm::vec2 center, float z)
{
    glm::mat4 zoom(1.0f);
    zoom[0][0] = scaleFactor;
    zoom[1][1] = scaleFactor;
    zoom[2][2] = scaleFactor;    
    _camera_center = glm::vec3(center, z);
    _camera = glm::lookAt(
        _camera_center,
        _camera_center + glm::vec3(0.0f, 0.0f, -1.0),
        glm::vec3(0.0f, 1.0f, 0.0f));
    _mvp = _projection * _camera;
}

void GLScence::translateCamera(glm::vec2 delta)
{
    _camera_center += glm::vec3(delta,0.0);
    _camera = glm::lookAt(
        _camera_center,
        _camera_center + glm::vec3(0.0f, 0.0f, -1.0),
        glm::vec3(0.0f, 1.0f, 0.0f));
    _mvp = _projection * _camera * glm::mat4(1.0);
}

void GLScence::resetZoom()
{
    scaleFactor = 1.0f;
    glm::mat4 zoom(1.0f);
    zoom[0][0] = scaleFactor;
    zoom[1][1] = scaleFactor;
    zoom[2][2] = scaleFactor;
    _mvp = _projection * _camera * glm::mat4(1.0) * zoom;
}

void GLScence::zoomIn()
{
    scaleFactor += 0.05f;
    if (scaleFactor >= 1.35)
        scaleFactor = 1.35;
    glm::mat4 zoom(1);
    zoom[0][0] = scaleFactor;
    zoom[1][1] = scaleFactor;
    zoom[2][2] = scaleFactor;
    _mvp = _projection * _camera * glm::mat4(1.0) * zoom;
}

void GLScence::zoomOut()
{
    scaleFactor -= 0.05f;
    if (scaleFactor <= 0.65)
        scaleFactor = 0.65;
    glm::mat4 zoom(1);
    zoom[0][0] = scaleFactor;
    zoom[1][1] = scaleFactor;
    zoom[2][2] = scaleFactor;
    _mvp = _projection * _camera * glm::mat4(1.0) * zoom;
}

glm::vec3 GLScence::getCameraCenter()
{
    return _camera_center;
}

glm::vec2 GLScence::screen_to_world(glm::vec2 screen, int w, int h)
{
    float percent_x = (2.0f * screen.x / width - 1) * width/2.0;
    int wx = _camera_center.x + percent_x;
    int wx_norm = std::floor(wx/ REALMZ_GRID_SIZE) * REALMZ_GRID_SIZE;

    float percent_y = (2.0f * screen.y / height - 1) * height / 2.0;
    int wy = -_camera_center.y + percent_y;
    int wy_norm = std::floor(wy / REALMZ_GRID_SIZE) * REALMZ_GRID_SIZE;

    if (wx_norm < 0)
        wx_norm = 0;

    if (wy_norm < 0)
        wy_norm = 0;

    if (w != 0 && wx_norm >= w)
        wx_norm = w;
    if (h != 0 && wy_norm >= h)
        wy_norm = h;

    // -1 * becose with draw y in negative \/ //
    return glm::vec2(wx_norm,-1.0 * wy_norm);
}

void GLScence::disableQuadShadow()
{
    quad_shadow_on = false;
}

void GLScence::enableQuadShadow()
{
    quad_shadow_on = true;
}

void GLScence::disableQuadSelection()
{
    quad_selection_on = false;
}

void GLScence::enableQuadSelection()
{
    quad_selection_on = true;
}

void GLScence::get_LeftTop_rightbot(glm::vec2 A, glm::vec2 B, glm::vec2 & leftTop, glm::vec2 & rightBot)
{
    if (A.y > B.y && A.x < B.x)
    {
        leftTop = A;
        rightBot = B;
    }
    if (A.y < B.y && A.x < B.x)
    {
        leftTop = glm::vec2(A.x, B.y);
        rightBot = glm::vec2(B.x, A.y);
    }
    if (A.y < B.y && A.x > B.x)
    {
        leftTop = B;
        rightBot = A;
    }
    if (A.y > B.y && A.x > B.x)
    {
        leftTop = glm::vec2(B.x, A.y);
        rightBot = glm::vec2(A.x, B.y);
    }

    if (A.x == B.x || A.y == B.y)
    {
        leftTop = A;
        rightBot = B;
    }
}

void GLScence::updateSelectionQuad(glm::vec2 A, glm::vec2 B, glm::vec4 colorRect, glm::vec4 colorBorder, float zCoord)
{
    glm::vec2 leftTop;
    glm::vec2 rightBot;
    get_LeftTop_rightbot(A, B, leftTop, rightBot);
    float borderSize = 4;
    _selection[1].updatePosition(glm::vec3(leftTop + glm::vec2(borderSize/2, -borderSize/2), 0.0), std::abs(leftTop.x - rightBot.x) -  borderSize, std::abs(leftTop.y - rightBot.y) - borderSize);
    _selection[0].updatePosition(glm::vec3(leftTop, 0.0), std::abs(leftTop.x - rightBot.x), std::abs(leftTop.y - rightBot.y));
    _selection[0].reset_textcoord(-1);
    _selection[1].reset_textcoord(-1);
    _selection[1].setColor(colorBorder);
    _selection[0].setColor(colorRect);

    _selection[0].setCoordZ(zCoord);
    _selection[1].setCoordZ(zCoord);
}

void GLScence::removeLightCylindergMapUI(int index)
{
    getQuad(index).setColor(glm::vec4(0.92, 0.32, 0.09, 1));
}

void GLScence::addLightCylindergMapUI(int index)
{
    getQuad(index).setColor(glm::vec4(1, 1, 1, 1));
}