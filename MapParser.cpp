#define _CRT_SECURE_NO_WARNINGS

#include "MapParser.h"
#include "TileLayer.h"

extern "C" MapParser * m_MapParser = MapParser_create();
extern "C" MapParser* MapParser_create() {
    MapParser* parser = (MapParser*)malloc(sizeof(MapParser));
    if (parser == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for MapParser\n");
        return NULL;
    }

    parser->m_MapDict = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);

    parser->Load = MapParser_Load;
    parser->Clean = MapParser_clean;
    parser->GetMap = MapParser_GetMap;
    parser->Parse = MapParser_Parse;
    parser->ParseTileset = MapParser_ParseTileset;
    parser->ParseTileLayer = MapParser_ParseTileLayer;

    return parser;
}

extern "C" gboolean MapParser_Load(MapParser* parser) {
    return parser->Parse(parser, (gchar*)"MAP", (gchar*)"assets/maps/level1.tmx");
}

extern "C" gboolean MapParser_Parse(MapParser * parser, gchar * id, gchar * source) {

    TiXmlDocument xml;
    xml.LoadFile(source);

    if (xml.Error())
    {
        std::cerr << "Failed to load: " << source << std::endl;
        return false;
    }

    TiXmlElement* root = xml.RootElement();
    int rowcount, colcount, tilesize = 0;
    root->Attribute("width", &colcount);
    root->Attribute("height", &rowcount);
    root->Attribute("tilewidth", &tilesize);

    TilesetList* tilesets = NULL;

    for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (e->Value() == std::string("tileset"))
        {
            Tileset tileset = parser->ParseTileset(e);
            Tileset* tilesetCopy = new Tileset(tileset); // Tạo bản sao của đối tượng Tileset
            tilesets = g_list_append(tilesets, tilesetCopy); // Thêm bản sao vào danh sách

            int t = 0;
        }
    }

    GameMap* gamemap = (GameMap*)g_malloc(sizeof(GameMap));
    {
        gamemap = GameMap_Init();
    }

    gamemap->m_MapLayers = NULL;

    for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (e->Value() == std::string("layer"))
        {
            TileLayer* tileLayer = parser->ParseTileLayer(e, tilesets, tilesize, rowcount, colcount);
            gamemap->m_MapLayers = g_list_append(gamemap->m_MapLayers, tileLayer);
        }
    }

    g_hash_table_insert(parser->m_MapDict, g_strdup(id), (GameMap*)(gamemap));
    return TRUE;
}

extern "C" Tileset MapParser_ParseTileset(TiXmlElement * xmlTileset) {
    Tileset tileset;
    strcpy(tileset.Name, (char*)xmlTileset->Attribute("name"));

    xmlTileset->Attribute("firstgid", &tileset.FirstID);

    xmlTileset->Attribute("tilecount", &tileset.TileCount);
    tileset.LastID = (tileset.FirstID + tileset.TileCount) - 1;

    xmlTileset->Attribute("columns", &tileset.ColCount);
    tileset.RowCount = tileset.TileCount / tileset.ColCount;
    xmlTileset->Attribute("tilewidth", &tileset.TileSize);

    TiXmlElement* image = xmlTileset->FirstChildElement();
    strcpy(tileset.Source, (char*)image->Attribute("source"));
    return tileset;
}

extern "C" TileLayer* MapParser_ParseTileLayer(TiXmlElement* xmlLayer, TilesetList* tilesets, int tilesize, int rowcount, int colcount) {
    TiXmlElement* data = nullptr;
    for (TiXmlElement* e = xmlLayer->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
        data = e;
        break;
    }

    std::string matrix(data->GetText());
    std::istringstream iss(matrix);
    std::string id;

    TileMap* tilemap = g_ptr_array_new();
    int index = 0;
    for (int row = 0; row < rowcount; row++) {
        for (int col = 0; col < colcount; col++) {
            getline(iss, id, ',');
            int value;
            std::stringstream convertor(id);
            convertor >> value;
            g_ptr_array_add(tilemap, GINT_TO_POINTER(value)); // store value as gpointer in tilemap
            index++;
        }
    }

    return {
        TileLayer_new(tilesize, rowcount, colcount, tilemap, tilesets)
    };
}

extern "C" GameMap * MapParser_GetMap(MapParser * parser, const gchar * id) {
    return (GameMap*)g_hash_table_lookup(parser->m_MapDict, id);
}

extern "C" void MapParser_clean(MapParser* parser) {
    g_return_if_fail(parser != NULL);

    g_hash_table_remove_all(parser->m_MapDict);
    g_hash_table_destroy(parser->m_MapDict);
    g_free(parser);
}