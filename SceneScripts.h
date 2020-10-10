#pragma once

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
    
    typedef struct ScriptDescriptor {
        ScriptDescriptor(std::string, std::string);
        std::string graphicalName;
        std::string IDJsonName;

    }ScriptDescriptor;

    class SceneScripts
    {
    public:
        SceneScripts();
        std::vector<GLRect> getQuads();

        void addScript(glm::vec2 xy, glm::vec2 dest, int floor, std::string graphicalName, std::string IDJsonName,
                       glm::vec4 RectColor = glm::vec4(0.5, 1, 0.25, 0.25), glm::vec4 borderColor = glm::vec4(0, 1, 1, 0.85), float borderSize = 1.0f);

    private:
        float _scriptLayerOverFloor;
        std::vector<GLRect> _areaQuads; // used by opengl to draw //
        std::vector<ScriptDescriptor> _descriptors;

    };
};

