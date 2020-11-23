#pragma once
#include <string>

class AppLoaderSettings
{
public:
    void load();

    std::string getMapNameToLoad();
    std::string getEditorType();
private:
    std::string mapToLoad;
    std::string editorType;
};

