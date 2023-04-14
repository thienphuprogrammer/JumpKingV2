#pragma once
#ifndef TILE_LAYER_H_
#define TILE_LAYER_H_

#include "Layer.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <glib.h>
#include <glib/garray.h>

#include "BaseObject.h"

typedef struct _Tileset Tileset;
struct _Tileset {
    int FirstID, LastID;
    int RowCount, ColCount;
    int TileCount, TileSize;
    char Name[100];
    char Source[100];
};

typedef GList TilesetList;
typedef GPtrArray TileMap;

typedef struct _TileLayer TileLayer;
struct _TileLayer {
    void (*Render)(TileLayer*, int, int, int);
    void (*Update)(TileLayer*);
    TileMap* (*GetTileMap)(TileLayer*);

    int m_TileSize;
    int m_RowCount, m_ColCount;
    TileMap *m_Tilemap;
    TilesetList *m_Tilesets;

    GHashTable* texture_map;
};

#ifdef __cplusplus
extern "C" {
#endif

    TileLayer* TileLayer_new(int tilesize, int rowcount, int colcount, TileMap* tilemap, TilesetList* tilesets);

#ifdef __cplusplus
}
#endif

TileLayer* TileLayer_new(int tilesize, int rowcount, int colcount, TileMap* tilemap, TilesetList* tilesets);
void TileLayer_free(TileLayer* layer);

void TileLayer_Render(TileLayer* layer, int tileID, int i, int j);
void TileLayer_Update(TileLayer* layer);
TileMap *TileLayer_GetTileMap(TileLayer* layer);

#endif // !TILE_LAYER_H_