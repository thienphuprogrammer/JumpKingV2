#define _CRT_SECURE_NO_WARNINGS
#include "TextObject.h"

TextObject TextObject_Init() {
    TextObject textObject;
    //textObject.str_val;
    textObject.width = 0;
    textObject.height = 0;
    textObject.text_color.r = 255;
    textObject.text_color.g = 255;
    textObject.text_color.b = 255;
    textObject.texture = NULL;
    textObject.rect = (SDL_Rect){ 0,0,0,0 };

    textObject.RenderText = TextObject_RenderText;
    textObject.SetColorByType = TextObject_SetColorByType;
    textObject.SetText = TextObject_SetText;
    return textObject;
}

void TextObject_destroy(TextObject* textObject) {
    if (textObject->texture != NULL) {
        SDL_DestroyTexture(textObject->texture);
        textObject->texture = NULL;
    }
}

bool TextObject_loadFromRenderedText(TextObject* textObject, TTF_Font* gFont, SDL_Renderer* screen) {
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textObject->str_val, textObject->text_color);
    if (textSurface != NULL) {
        //Create texture from surface pixels
        textObject->texture = SDL_CreateTextureFromSurface(screen, textSurface);
        if (textObject->texture != NULL) {
            //Get image dimensions
            textObject->width = textSurface->w;
            textObject->height = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return true;
}

void TextObject_free(TextObject* textObject) {
    if (textObject->texture != NULL) {
        SDL_DestroyTexture(textObject->texture);
        textObject->texture = NULL;
    }
}

void TextObject_setColorRGB(TextObject* textObject, Uint8 red, Uint8 green, Uint8 blue) {
    textObject->text_color.r = red;
    textObject->text_color.g = green;
    textObject->text_color.b = blue;
}

void TextObject_SetColorByType(TextObject* textObject, int type) {
    if (type == RED_TEXT) {
        SDL_Color color = { 255, 0, 0 };
        textObject->text_color = color;
    }
    else if (type == WHITE_TEXT) {
        SDL_Color color = { 255, 255, 255 };
        textObject->text_color = color;
    }
    else {
        SDL_Color color = { 0, 0, 0 };
        textObject->text_color = color;
    }
}

void TextObject_RenderText(TextObject* textObject, SDL_Renderer* screen, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, textObject->width, textObject->height };

    //Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(screen, textObject->texture, clip, &renderQuad, angle, center, flip);
    textObject->rect = renderQuad;
}
void TextObject_SetText(TextObject* text, const char* text_str) {
    strcpy(text->str_val, (text_str));
}
SDL_Rect TextObject_GetRect(TextObject* text) {
    return text->rect;
}