#include "GameMap.h"
#include "TileLayer.h"

GameMap* GameMap_Init()
{
    GameMap* map = (GameMap*)(malloc)(sizeof(GameMap));
    map->Render = GameMap_Render;
    map->Update = GameMap_Update;
    map->GetMapLayers = GameMap_GetMapLayers;
    map->Free = GameMap_Free;
    map->m_MapLayers = g_ptr_array_new();
    return map;
}
void GameMap_Free(GameMap* map)
{
    for (GList* iter = map->m_MapLayers; iter != NULL; iter = g_list_next(iter)) {
        TileLayer* layer = (TileLayer*)iter->data;
        TileLayer_free(layer);
    }
    // Giải phóng bộ nhớ của mảng chứa các lớp bản đồ
    g_list_free(map->m_MapLayers);
    map->m_MapLayers = NULL;
    if (map != NULL)
    {
        free(map);
    }
}

void GameMap_Render(GameMap* map) {
    for (GList* layer_node = map->m_MapLayers; layer_node != NULL; layer_node = layer_node->next) {
        TileLayer* layer = (TileLayer*)layer_node->data;

        //layer->Render(layer);
    }
}

void GameMap_Update(GameMap* map) {
    for (GList* layer_node = map->m_MapLayers; layer_node != NULL; layer_node = layer_node->next) {
        TileLayer* layer = (TileLayer*)layer_node->data;
        layer->Update(layer);
    }
}

GPtrArray* GameMap_GetMapLayers(const GameMap* map) {
    return map->m_MapLayers;
}