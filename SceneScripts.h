#pragma once

#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "GLRect.h"
#include "DrawingFunctions.h"


// The script is composed by
// GRAPHICAL AREA RECT
// GRAPHICAL NAME
// script id NAME FROM JSON script FILE

namespace Scripts {
    
    typedef struct SceneScript{
        SceneScript();
        SceneScript(glm::vec2 xy, glm::vec2 dest, float floor,
                    glm::vec4 RectColor = glm::vec4(0.5, 1, 0.25, 0.25), glm::vec4 borderColor = glm::vec4(1, 1, 0, 0.85), float borderSize = 1.0f,
                    glm::vec2 offset = glm::vec2(3,3));
        glm::vec2 leftTopPoint;
        glm::vec2 rightBotPoint;
        glm::vec4 borderColor;
        glm::vec4 rectColor;
        float borderSize;
        float floor;
        float zRect;
        float zBorder;
        float zText;
        std::string name;
        glm::vec2 offsetText;
    }SceneScript;

    class ContainerSceneScript
    {
    public:
        ContainerSceneScript();
        std::vector<GLRect> getQuads();
        void addScript(Scripts::SceneScript);
        const std::map < std::string, Scripts::SceneScript>& getScripts();

    private:
        static int _coutScripts;
        std::vector<GLRect> _areaQuads; // used by opengl to draw //        
        std::map < std::string, Scripts::SceneScript> _scripts;
    };
};

