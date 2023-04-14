#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

extern SDL_Window* g_windows;
extern SDL_Renderer* g_screen;
extern SDL_Event g_event;

//Screen
extern const int FRAME_PER_SECOND;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

extern const int COLOR_KEY_R;
extern const int COLOR_KEY_G;
extern const int COLOR_KEY_B;

extern const int RENDERER_DRAW_COLOR;

#define BLANK_TILE 0
#define TILE_SIZE 32

#define MAX_MAP_X 40
#define MAX_MAP_Y 200

#define STATE_MONEY 4

typedef struct Input {
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;
} Input;

typedef struct Map {
	int start_x_;
	int start_y_;

	int max_x_;
	int max_y_;

	int tile[MAX_MAP_Y][MAX_MAP_X];
	char* file_name;
} Map;

#endif // !COMMON_FUNCTION_H_
