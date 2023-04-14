#include "CommonFunction.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "GameMap.h"
#include "TextObject.h"

const int MENU_START = 0;
const int MENU_QUIT = 1;
const int RENDER_DRAW_COLOR = 0xff;

BaseObject g_background;
TTF_Font* g_font = NULL;
BaseObject g_background_menu;

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

    g_windows = SDL_CreateWindow("Game jump king dream",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, 
        SCREEN_HEIGHT,
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

    //Init TTF
    if (TTF_Init() < 0)
        return false;
    //size
    g_font = TTF_OpenFont("fonts/EnterCommand.ttf", 70);
    return success;
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg(&g_background, "img//BG.png", g_screen);
    if (ret == false) return false;
    bool ret_menu = g_background_menu.LoadImg(&g_background_menu, "img/background_menu.jpg", g_screen);
    if (ret_menu == false)
        return false;


    return true;
}

void close()
{
    g_background.Free(&g_background);
    g_background.Destroy(&g_background);
    g_background_menu.Free(&g_background_menu);

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_windows);
    g_windows = NULL;
    TTF_CloseFont(g_font);


    IMG_Quit();
    SDL_Quit();
}

int selectMenu()
{

    TextObject text_start_ = TextObject_Init(); //TextObject lop doi tuong
    TextObject text_quit_ = TextObject_Init(); //TextObject lop doi tuong


    TextObject_SetText(&text_start_, "Start!!");
    TextObject_SetText(&text_quit_, "Quit!!");
    text_start_.SetColorByType(&text_start_, BLACK_TEXT);
    text_start_.SetColorByType(&text_quit_, BLACK_TEXT);

    int mouseX_button = 0, mouseY_button = 0, mouseX = 0, mouseY = 0;

    bool running = true;
    while (running)
    {
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        g_background_menu.Render(&g_background_menu, g_screen, NULL);

        TextObject_loadFromRenderedText(&text_start_, g_font, g_screen);
        text_start_.RenderText(&text_start_, g_screen, 840, 220, NULL, 0.0, NULL, SDL_FLIP_NONE);

        TextObject_loadFromRenderedText(&text_quit_, g_font, g_screen);
        text_quit_.RenderText(&text_quit_, g_screen, 852, 290, NULL, 0.0, NULL, SDL_FLIP_NONE);


        while (SDL_PollEvent(&g_event) != 0)
        {
            switch (g_event.type)
            {
            case SDL_QUIT:
                return MENU_QUIT;
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouseX_button = g_event.button.x;
                mouseY_button = g_event.button.y;
                {
                    SDL_Point mouse_point_button = { mouseX_button, mouseY_button };
                    SDL_Rect pos_text_start_button = TextObject_GetRect(&text_start_);
                    SDL_Rect pos_text_quit_button = TextObject_GetRect(&text_quit_);
                    if (SDL_PointInRect(&mouse_point_button, &pos_text_start_button))
                    {
                        TextObject_SetColorByType(&text_start_, RED_TEXT);
                        return 0;
                    }
                    else if (SDL_PointInRect(&mouse_point_button, &pos_text_quit_button))
                    {
                        TextObject_SetColorByType(&text_quit_, RED_TEXT);
                        return 1;
                    }
                }
                break;

            case SDL_MOUSEMOTION:
                mouseX = g_event.button.x;
                mouseY = g_event.button.y;
                {
                    SDL_Point mouse_motton = { mouseX, mouseY };
                    SDL_Rect pose_text_start_motton = TextObject_GetRect(&text_start_);
                    SDL_Rect pose_text_quit_motton = TextObject_GetRect(&text_quit_);
                    if (SDL_PointInRect(&mouse_motton, &pose_text_start_motton))
                    {
                        TextObject_SetColorByType(&text_start_, RED_TEXT);
                    }
                    else
                    {
                        TextObject_SetColorByType(&text_start_, BLACK_TEXT);
                    }
                    if (SDL_PointInRect(&mouse_motton, &pose_text_quit_motton))
                    {
                        TextObject_SetColorByType(&text_quit_, RED_TEXT);
                    }
                    else
                    {
                        TextObject_SetColorByType(&text_quit_, BLACK_TEXT);
                    }
                }
                break;
            }
        }

        SDL_RenderPresent(g_screen);
    }

    return 0;
}


#undef main
int main(int argc, char* argv[]) {

    ImpTimer fps_timer = ImpTimer_Create();

    if (!InitData()) return -1;

    g_background = *BaseObject_Create();
    g_background_menu = *BaseObject_Create();

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
    if (selectMenu() == 1)
    {
        is_quit = true;
    }
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
