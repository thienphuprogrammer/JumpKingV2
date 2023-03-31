#ifndef GAME_MAP_H_
#define GAME_MAP_H_

#define _CRT_SECURE_NO_WARNINGS
#include "CommonFunction.h"
#include "BaseObject.h"

#define MAX_TILES 20

typedef struct sTileMat TileMat;
struct sTileMat
{
    BaseObject base_object;
};

typedef struct sGameMap GameMap;
struct sGameMap
{
    Map game_map_;
    TileMat tile_mat[MAX_TILES];

    void (*Destroy) (GameMap*);
    void (*LoadMap) (GameMap*, char*);
    void (*LoadTiles) (GameMap*, SDL_Renderer*);
    void (*DrawMap) (GameMap*, SDL_Renderer*);
    Map (*GetMap) (GameMap*);
    void (*SetMap) (GameMap* , const Map* gMap);
};

GameMap GameMap_Create();
void GameMap_Destroy(GameMap* obj);
void GameMap_LoadMap(GameMap* obj, char* name);
void GameMap_LoadTiles(GameMap* obj, SDL_Renderer* screen);
void GameMap_DrawMap(GameMap* obj, SDL_Renderer* des);
Map GameMap_GetMap(GameMap* obj);
void GameMap_SetMap(GameMap* obj, const Map* gMap);

#endif // !GAME_MAP_H_