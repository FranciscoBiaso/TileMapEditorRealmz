#include "SceneScripts.h"


Scripts::ScriptDescriptor::ScriptDescriptor(std::string graphicalName, std::string IDJsonName)
{
    this->graphicalName = graphicalName;
    this->IDJsonName = IDJsonName;
}

std::vector<GLQuad> Scripts::SceneScripts::getQuads()
{
    return _areaQuads;
}

Scripts::SceneScripts::SceneScripts()
{
    _scriptLayerOverFloor = 0.1f/10.0f;
}

void Scripts::SceneScripts::addScript(glm::vec2 xy, float size, int floor, std::string graphicalName, std::string IDJsonName, glm::vec4 color)
{
    glm::vec2 leftTop;
    glm::vec2 rightBot;
    graphics::get_LeftTop_rightbot(xy, xy + glm::vec2(size, -size), leftTop, rightBot);
    GLQuad quad(glm::vec3(leftTop, 2 - _scriptLayerOverFloor), size, color);    
    quad.reset_textcoord(-1); // to not apply texture //
    _areaQuads.push_back(quad);
    _descriptors.push_back(ScriptDescriptor(graphicalName, IDJsonName));

}