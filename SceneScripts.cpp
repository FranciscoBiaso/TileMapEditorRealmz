#include "SceneScripts.h"

int Scripts::ContainerSceneScript::_coutScripts = 0;

Scripts::SceneScript::SceneScript()
{}

Scripts::SceneScript::SceneScript(glm::vec2 leftTopPoint, glm::vec2 rightBotPoint, float floor,
                      glm::vec4 RectColor, glm::vec4 borderColor, float borderSize, glm::vec2 offset)
    :leftTopPoint(leftTopPoint),rightBotPoint(rightBotPoint), 
    borderColor(borderColor), rectColor(RectColor),
    borderSize(borderSize), floor(floor),offsetText(offset),
    scriptToLoad("empty")
{
    zRect = floor - 9 * 0.1f / 10.0;
    zBorder = floor - 8 * 0.1f / 10.0;
    zText = floor - 7 * 0.1f / 10.0;
}

std::vector<GLRect> Scripts::ContainerSceneScript::getQuads()
{
    return _areaQuads;
}

Scripts::ContainerSceneScript::ContainerSceneScript() : _showScriptsTexts(true), _showScriptsRects(true)
{}

void Scripts::ContainerSceneScript::addScript(Scripts::SceneScript script)
{
    float borderLayer = 8 * 0.1f / 10.0f;
    glm::vec2 leftTop = script.leftTopPoint;
    glm::vec2 rightBot = script.rightBotPoint;    
    float zRect = script.zRect;
    float zBorder = script.zBorder;
    //float zText = script.zText;
    float borderSize = script.borderSize;
    glm::vec4 borderColor = script.borderColor;
    glm::vec4 rectColor = script.rectColor;
    
    // top border
    GLRect quad(glm::vec3(leftTop, zBorder), glm::vec3(leftTop + glm::vec2(1,0) * glm::abs(rightBot.x- leftTop.x) - glm::vec2(0, borderSize), zBorder), borderColor);
    quad.reset_textcoord(-1); // to not apply texture //
    _areaQuads.push_back(quad);
    // left border
    quad = GLRect(glm::vec3(leftTop, zBorder), glm::vec3(leftTop - glm::vec2(0, 1) * glm::abs(rightBot.y - leftTop.y) + glm::vec2(borderSize, 0), zBorder), borderColor);
    quad.reset_textcoord(-1); // to not apply texture //
    _areaQuads.push_back(quad);
    // right border
    quad = GLRect(glm::vec3(leftTop + glm::vec2(1, 0) * glm::abs(rightBot.x - leftTop.x) - glm::vec2(borderSize, 0)/*POINT D*/, zBorder), glm::vec3(leftTop + (rightBot -leftTop), zBorder), borderColor);
    quad.reset_textcoord(-1); // to not apply texture //
    _areaQuads.push_back(quad);
    // bot border
    quad = GLRect(glm::vec3(leftTop - glm::vec2(0, 1) * glm::abs(rightBot.y - leftTop.y) + glm::vec2(0, borderSize)/*POINT B*/, zBorder), glm::vec3(leftTop + (rightBot - leftTop), zBorder), borderColor);
    quad.reset_textcoord(-1); // to not apply texture //
    _areaQuads.push_back(quad);     

    quad = GLRect(glm::vec3(leftTop, zRect), glm::vec3(rightBot, zRect), rectColor);
    quad.reset_textcoord(-1); // to not apply texture //
    _areaQuads.push_back(quad);  

    _coutScripts++;
    script.name = std::to_string(_coutScripts);
    _scripts[script.name] = script;
}


void Scripts::ContainerSceneScript::delScript(std::string scriptID)
{
    _scripts.erase(scriptID);
    _coutScripts--;
}

std::map <std::string, Scripts::SceneScript>& Scripts::ContainerSceneScript::getScripts()
{
    return _scripts;
}

void Scripts::ContainerSceneScript::setShowScriptsTexts(bool value)
{
    _showScriptsTexts = value;
}

bool Scripts::ContainerSceneScript::areScriptsTextsVisibles()
{
    return _showScriptsTexts;
}

void Scripts::ContainerSceneScript::setShowScriptsRects(bool value)
{
    _showScriptsRects = value;
}

bool Scripts::ContainerSceneScript::areScriptsRectsVisibles()
{
    return _showScriptsRects;
}

void Scripts::ContainerSceneScript::cleanScriptRects()
{
    _areaQuads.clear();    
}

void Scripts::ContainerSceneScript::cleanScripts()
{
    _coutScripts = 0;
    _scripts.clear();
}

void Scripts::ContainerSceneScript::updateRectColor(std::string scriptID, glm::vec4 color)
{
    auto it = _scripts.find(scriptID);
    if (it == _scripts.end())
        return;
        
    _scripts[scriptID].rectColor = color;
    cleanScriptRects();
    auto map = getScripts();
    cleanScripts();
    for (auto it = map.begin(); it != map.end(); it++)
        addScript(it->second);
}


std::string Scripts::ContainerSceneScript::getSelectedScriptID()
{
    return _selectedScriptID;
}
void Scripts::ContainerSceneScript::setSelectedScriptID(std::string ID)
{
    _selectedScriptID = ID;
}