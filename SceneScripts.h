#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "GLQuad.h"
#include "DrawingFunctions.h"

// The script is composed by
// GRAPHICAL AREA QUAD
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
        std::vector<GLQuad> getQuads();

        void addScript(glm::vec2 xy, float size, int floor, std::string graphicalName, std::string IDJsonName, glm::vec4 color);

    private:
        float _scriptLayerOverFloor;
        std::vector<GLQuad> _areaQuads; // used by opengl to draw //
        std::vector<ScriptDescriptor> _descriptors;

    };
};

