#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include <stdbool.h>
#include <string.h>
#include "CommonFunction.h"

typedef struct TextObject TextObject;
typedef enum TextColor TextColor;
struct TextObject
{
	char str_val[80];
	SDL_Color text_color;
	SDL_Texture* texture;
	int width;
	int height;
	SDL_Rect rect;

	void (*RenderText) (TextObject*, SDL_Renderer*, int, int, SDL_Rect*, double, SDL_Point*, SDL_RendererFlip);
	void (*SetColorByType) (TextObject*, int);
	void (*SetText) (TextObject*, const char*);
};

enum TextColor
{
	RED_TEXT = 0,
	WHITE_TEXT = 1,
	BLACK_TEXT = 2,
};

TextObject TextObject_Init();
void TextObject_Destroy(TextObject* text);
bool TextObject_LoadFromFile(TextObject* text, const char* path);
bool TextObject_loadFromRenderedText(TextObject* text, TTF_Font* gFont, SDL_Renderer* screen);
void TextObject_Free(TextObject* text);
void TextObject_SetColor(TextObject* text, Uint8 red, Uint8 green, Uint8 blue);
void TextObject_SetColorByType(TextObject* text, int type);
void TextObject_RenderText(TextObject* text, SDL_Renderer* screen, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);
int TextObject_GetWidth(TextObject* text);
int TextObject_GetHeight(TextObject* text);
void TextObject_SetText(TextObject* text, const char* text_str);
SDL_Rect TextObject_GetRect(TextObject* text);
const char* TextObject_GetText(TextObject* text);

#endif
