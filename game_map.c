#include "game_map.h"
#include "GameMap.h"
#include <glib.h>

GameMapp GameMapp_Create()
{
    GameMapp obj;
    for (int i = 0; i < MAX_TILES; i++) {
        obj.tile_mat[i].base_object = *BaseObject_Create();
    }
    obj.Destroy = GameMapp_Destroy;

    obj.LoadMap = GameMapp_LoadMap;
    obj.LoadTiles = GameMapp_LoadTiles;
    obj.DrawMap = GameMapp_DrawMap;

    obj.GetMap = GameMapp_GetMap;
    obj.SetMap = GameMapp_SetMap; 
    return obj;
}
void GameMapp_Destroy(GameMapp* obj)
{
    /*if (obj) {
        if (obj->tile_mat) {
            for (int i = 0; i < MAX_TILES; i++) {
                obj->tile_mat[i].base_object.Destroy(&obj->tile_mat[i].base_object);
            }
        }
    }*/
}
Map GameMapp_GetMap(GameMapp* obj) { return obj->game_map_; }
void GameMapp_SetMap(GameMapp* obj, const Map gMap) { obj->game_map_ = gMap; }

void GameMapp_LoadMap(GameMapp* obj, TileLayer* TileMap) {

    obj->game_map_.max_x_ = 0;
    obj->game_map_.max_y_ = 0;
    for (int i = 0; i < MAX_MAP_Y; i++) {
        for (int j = 0; j < MAX_MAP_X; j++) {
            obj->game_map_.tile[i][j] = (int)g_ptr_array_index(TileMap->m_Tilemap, i * MAX_MAP_X + j);
            int val = obj->game_map_.tile[i][j];
            if (val > 0) {
                if (j > obj->game_map_.max_x_) {
                    obj->game_map_.max_x_ = j;
                }
                if (i > obj->game_map_.max_y_) {
                    obj->game_map_.max_y_ = i;
                }
            }
        }
    }
    obj->game_map_.max_x_ = (obj->game_map_.max_x_ + 1) * TILE_SIZE;
    obj->game_map_.max_y_ = (obj->game_map_.max_y_ + 1) * TILE_SIZE;

    obj->game_map_.start_x_ = 0;
    obj->game_map_.start_y_ = (MAX_MAP_Y - 10) * TILE_SIZE;
}

void GameMapp_LoadTiles(GameMapp* obj, SDL_Renderer* screen) {
    char file_img[30];
    FILE* fp = NULL;

    for (int i = 0; i < MAX_TILES; i++) {
        sprintf(file_img, "map/%d.png", i);

        fopen_s(&fp, file_img, "rb");
        if (fp == NULL) {
            continue;
        }
        fclose(fp);
        obj->tile_mat[i].base_object.LoadImg(&obj->tile_mat[i].base_object, file_img, screen);
    }
}

void GameMapp_DrawMap(GameMapp* obj, SDL_Renderer* des, GameMap* LeMap) {
    for (GList* layer_node = LeMap->m_MapLayers; layer_node != NULL; layer_node = layer_node->next) {
        TileLayer* layer = (TileLayer*)layer_node->data;
        int x1 = 0;
        int x2 = 0;

        int y1 = 0;
        int y2 = 0;

        int map_x = 0;
        int map_y = 0;

        map_x = obj->game_map_.start_x_ / TILE_SIZE;
        x1 = (obj->game_map_.start_x_ % TILE_SIZE) * -1;
        x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

        map_y = obj->game_map_.start_y_ / TILE_SIZE;
        y1 = (obj->game_map_.start_y_ % TILE_SIZE) * -1;
        y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);


       /* for (int i = 0; i < layer->m_RowCount; i++)
        {
            for (int j = 0; j < layer->m_ColCount; j++)
            {
                int tileID = ((int)g_ptr_array_index(layer->m_Tilemap, i * layer->m_ColCount + j));
                printf("%d ", tileID);
            }
            printf("\n");
        }
        printf("\n");*/

        for (int i = y1; i < y2; i += TILE_SIZE) {
            map_x = obj->game_map_.start_x_ / TILE_SIZE;
            for (int j = x1; j < x2; j += TILE_SIZE) {
                int val = obj->game_map_.tile[map_y][map_x];
                if (val > 0) {
                    /*  obj->tile_mat[val].base_object.SetRect(&obj->tile_mat[val].base_object, j, i);
                      obj->tile_mat[val].base_object.Render(&obj->tile_mat[val].base_object, des, NULL);*/


                    layer->Render(layer, val, i, j);
                }
                map_x++;
            }
            map_y++;
        }
    }
}