#pragma once
#include <string>
class AppLoaderSettings
{
public:
    void load();

    int getMapWidth();
    int getMapHeight();
    int getMapLevels();
    std::string getMapNameToLoad();
private:
    int mapWidth;
    int mapHeight;
    int mapLevels;
    std::string mapToLoad;
};

