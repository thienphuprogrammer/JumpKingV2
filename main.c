#include "CommonFunction.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "GameMap.h"

#ifdef __cplusplus
extern "C"
#include "MapParser.hpp"
#endif
typedef struct _MapParser MapParser;
MapParser* m_MapParser;
gboolean MapParser_Load(MapParser* parser);
void MapParser_clean(MapParser* parser);
GameMap* MapParser_GetMap(MapParser* parser, const gchar* id);
void MapParser_clean(MapParser* parser);

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_windows = SDL_CreateWindow("Game Cpp SDL 2.0",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (g_windows == NULL) success = false;
    else
    {
        g_screen = SDL_CreateRenderer(g_windows, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL) success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen,
                RENDERER_DRAW_COLOR,
                RENDERER_DRAW_COLOR,
                RENDERER_DRAW_COLOR,
                RENDERER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
            {
                /*std::cout << "Error: " << IMG_GetError() << std::endl;*/
                success = false;
            }
        }
    }
    return success;
}

BaseObject g_background;

bool LoadBackground()
{
    bool ret = g_background.LoadImg(&g_background, "img//BG.png", g_screen);
    if (ret == false) return false;
    return true;
}

void close()
{
    g_background.Free(&g_background);
    g_background.Destroy(&g_background);
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_windows);
    g_windows = NULL;

    IMG_Quit();
    SDL_Quit();
}

#undef main
int main(int argc, char* argv[]) {

    ImpTimer fps_timer = ImpTimer_Create();
    g_background = *BaseObject_Create();

    if (!InitData()) return -1;
    if (!LoadBackground(&g_background)) return -1;

    MainObject p_player = MainObject_Create();
    p_player.LoadImg(&p_player, "img/MainObject/MoveRight.png", g_screen);
    p_player.set_clips(&p_player);

    GameMap* m_LevelMap;

    if (!MapParser_Load(m_MapParser)) {
        printf("Failed to load map\n");
    }
    m_LevelMap = MapParser_GetMap(m_MapParser, "MAP");
    TileLayer* liMap = m_LevelMap->m_MapLayers->data;

    GameMapp game_map = GameMapp_Create();
    //char map[] = "map/map01.dat";
    game_map.LoadMap(&game_map, liMap);
    //game_map.LoadTiles(&game_map, g_screen);

    bool is_quit = false;
    while (!is_quit)
    {
        fps_timer.start(&fps_timer);

        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }

            p_player.HandleInputAction(&p_player, g_event, g_screen);
        }

        SDL_RenderClear(g_screen);
        g_background.SetRect(&g_background, 0, 0);
        g_background.Render(&g_background, g_screen, NULL);
        g_background.SetRect(&g_background, g_background.GetRect(&g_background).w, 0);
        g_background.Render(&g_background, g_screen, NULL);

        Map map_data = game_map.GetMap(&game_map);

        //Player
        p_player.DoPlayer(&p_player, &map_data);
        p_player.SetMapXY(&p_player, map_data.start_x_, map_data.start_y_);
        p_player.Show(&p_player, g_screen);

        //Map
        game_map.SetMap(&game_map, map_data);
        game_map.DrawMap(&game_map, g_screen, m_LevelMap);

        SDL_RenderPresent(g_screen);

        //SET FPS FOR GAME
        int real_imp_time = fps_timer.get_ticks(&fps_timer);
        int time_one_frame = 1000 / FRAME_PER_SECOND;

        if (real_imp_time < time_one_frame) {
            int delay_time = time_one_frame - real_imp_time;
            if (delay_time >= 0)
                SDL_Delay(delay_time);
        }
    }
    close();
    p_player.Destroy(&p_player);
    game_map.Destroy(&game_map);
    //m_LevelMap->Free(m_LevelMap);
    //MapParser_clean(m_MapParser);
    return 0;
}
