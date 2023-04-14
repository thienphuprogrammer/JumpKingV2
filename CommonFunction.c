#include "CommonFunction.h"

SDL_Window* g_windows = NULL;
SDL_Renderer* g_screen = NULL;
SDL_Event g_event;


const int FRAME_PER_SECOND = 55;
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDERER_DRAW_COLOR = 0xff;
