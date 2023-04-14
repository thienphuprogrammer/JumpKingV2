#ifndef MAP_PARSER_H_
#define MAP_PARSER_H_

#include <iostream>
#include <glib.h>
extern "C" {
#include "GameMap.h"
}
#include "TileLayer.h"

#include "tinyxml/tinyxml.h"

typedef struct _MapParser MapParser;
struct _MapParser {
    gboolean(*Load)(MapParser*);
    GameMap* (*GetMap)(MapParser*, const char*);
    gboolean(*Parse)(MapParser*, gchar*, gchar*);
    Tileset (*ParseTileset)(TiXmlElement*);
    TileLayer* (*ParseTileLayer)(TiXmlElement*, TilesetList*, int, int, int);

    void (*Clean)(MapParser*);
    GHashTable* m_MapDict;
};


#ifdef __cplusplus
extern "C" {
#endif

    MapParser* MapParser_create();
    MapParser* m_MapParser;

#ifdef __cplusplus
}
#endif

extern "C" MapParser * MapParser_create();
extern "C" void MapParser_clean(MapParser* parser);

extern "C" gboolean MapParser_Load(MapParser* parser);
extern "C" gboolean MapParser_Parse(MapParser* parser, gchar* id, gchar* source);
extern "C" Tileset MapParser_ParseTileset(TiXmlElement * xmlTileset);
extern "C" TileLayer * MapParser_ParseTileLayer(TiXmlElement * xmlLayer, TilesetList * tilesets, int tilesize, int rowcount, int colcount);
extern "C" GameMap * MapParser_GetMap(MapParser * parser, const gchar * id);

#endif /* MAP_PARSER_H_ */
