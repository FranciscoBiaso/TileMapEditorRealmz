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
    // layer 9 
    _scriptLayerOverFloor = 9 * 0.1f/10.0f;
}

void Scripts::SceneScripts::addScript(glm::vec2 leftTop, glm::vec2 rightBot, int floor, std::string graphicalName, std::string IDJsonName, 
    glm::vec4 rectColor, glm::vec4 borderColor, float borderSize)
{
    float borderLayer = 8 * 0.1f / 10.0f;
    // top border
    GLQuad quad(glm::vec3(leftTop, floor - borderLayer), glm::vec3(leftTop + glm::vec2(1,0) * glm::abs(rightBot.x- leftTop.x) - glm::vec2(0, borderSize), floor - borderLayer), borderColor);
    quad.reset_textcoord(-1); // to not apply texture //
    _areaQuads.push_back(quad);
    // left border
    quad = GLQuad(glm::vec3(leftTop, floor - borderLayer), glm::vec3(leftTop - glm::vec2(0, 1) * glm::abs(rightBot.x - leftTop.x) + glm::vec2(borderSize, 0), floor - borderLayer), borderColor);
    quad.reset_textcoord(-1); // to not apply texture //
    _areaQuads.push_back(quad);
    // right border
    quad = GLQuad(glm::vec3(leftTop + glm::vec2(1, 0) * glm::abs(rightBot.x - leftTop.x) - glm::vec2(borderSize, 0)/*POINT D*/, floor - borderLayer), glm::vec3(leftTop + (rightBot -leftTop), floor - borderLayer), borderColor);
    quad.reset_textcoord(-1); // to not apply texture //
    _areaQuads.push_back(quad);
    // bot border
    quad = GLQuad(glm::vec3(leftTop - glm::vec2(0, 1) * glm::abs(rightBot.y - leftTop.y) + glm::vec2(0, borderSize)/*POINT B*/, floor - borderLayer), glm::vec3(leftTop + (rightBot - leftTop), floor - borderLayer), borderColor);
    quad.reset_textcoord(-1); // to not apply texture //
    _areaQuads.push_back(quad);
    
 

    quad = GLQuad(glm::vec3(leftTop, floor - _scriptLayerOverFloor), glm::vec3(rightBot, floor - _scriptLayerOverFloor), rectColor);
    quad.reset_textcoord(-1); // to not apply texture //
    _areaQuads.push_back(quad);

    _descriptors.push_back(ScriptDescriptor(graphicalName, IDJsonName));
}