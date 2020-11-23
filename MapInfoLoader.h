#pragma once
class MapInfoLoader
{
public:
    void load();
    int getMapWidth();
    int getMapHeight();
    int getMapLevels();
private:
    int mapWidth;
    int mapHeight;
    int mapLevels;
};