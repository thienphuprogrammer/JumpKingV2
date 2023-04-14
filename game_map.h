#ifndef GAME_MAP_H_
#define GAME_MAP_H_

#define _CRT_SECURE_NO_WARNINGS
#include "CommonFunction.h"
#include "BaseObject.h"
#include "GameMap.h"

#define MAX_TILES 20

typedef struct sTileMat TileMat;
struct sTileMat
{
    BaseObject base_object;
};

typedef struct sGameMap GameMapp;
struct sGameMap
{
    Map game_map_;
    TileMat tile_mat[MAX_TILES];

    void (*Destroy) (GameMapp*);
    void (*LoadMap) (GameMapp*, TileLayer*);
    void (*LoadTiles) (GameMapp*, SDL_Renderer*);
    void (*DrawMap) (GameMapp*, SDL_Renderer*, GameMap*);
    Map (*GetMap) (GameMapp*);
    void (*SetMap) (GameMapp*, const Map);
};

GameMapp GameMapp_Create();
void GameMapp_Destroy(GameMapp* obj);
void GameMapp_LoadMap(GameMapp* obj, TileLayer* TileMap);
void GameMapp_LoadTiles(GameMapp* obj, SDL_Renderer* screen);
void GameMapp_DrawMap(GameMapp* obj, SDL_Renderer* des, GameMap* LeMap);
Map GameMapp_GetMap(GameMapp* obj);
void GameMapp_SetMap(GameMapp* obj, const Map gMap);

#endif // !GAME_MAP_H_