#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "CommonFunction.h"

typedef struct sBaseObject BaseObject;
struct sBaseObject
{
	SDL_Texture* p_object_;
	SDL_Rect rect_;

	void (*Destroy) (BaseObject*);
	void (*SetRect) (BaseObject*, const int, const int);
	SDL_Rect(*GetRect) (const BaseObject*);
	SDL_Texture* (*GetObject) (const BaseObject*);

	bool (*LoadImg) (BaseObject*, const char*, SDL_Renderer*);
	void (*Render) (BaseObject*, SDL_Renderer*, const SDL_Rect*);
	void (*DrawTile)(SDL_Renderer*, SDL_Texture*, int, int, int, int, int, SDL_RendererFlip);

	void (*Free) (BaseObject*);
};

BaseObject* BaseObject_Create();
void BaseObject_Destroy(BaseObject* obj);
void BaseObject_SetRect(BaseObject* obj, const int x, const int y);

SDL_Rect BaseObject_GetRect(const BaseObject* obj);
SDL_Texture* BaseObject_GetObject(const BaseObject* obj);

bool BaseObject_LoadImg(BaseObject* obj, const char* path, SDL_Renderer* screen);
void BaseObject_DrawTile(SDL_Renderer* des, SDL_Texture* texture, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip);

void BaseObject_Render(BaseObject* obj, SDL_Renderer* des, const SDL_Rect* clip);
void BaseObject_Free(BaseObject* obj);

#endif // !BASE_OBJECT_H_