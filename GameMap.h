#ifndef GAMEMAP_H_
#define GAMEMAP_H_

#include "Layer.h"
#include <glib.h>

typedef struct GameMap GameMap;
struct GameMap {
    void (*Render)(GameMap*);
    void (*Update)(GameMap*);
    void (*Free)(GameMap*);

    GPtrArray* (*GetMapLayers)(const GameMap*);
    GList* m_MapLayers;
};

GameMap* GameMap_Init();
void GameMap_Free(GameMap* map);

void GameMap_Render(GameMap* map);
void GameMap_Update(GameMap* map);

GPtrArray* GameMap_GetMapLayers(const GameMap* map);

#endif /* GAMEMAP_H_ */
