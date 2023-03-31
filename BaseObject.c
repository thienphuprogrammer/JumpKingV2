#include "BaseObject.h"

BaseObject BaseObject_Create()
{
	BaseObject obj;
	obj.p_object_ = NULL;
	obj.rect_.x = 0;
	obj.rect_.y = 0;
	obj.rect_.w = 0;
	obj.rect_.h = 0;
	obj.Destroy = BaseObject_Destroy;
	obj.SetRect = BaseObject_SetRect;
	obj.GetRect = BaseObject_GetRect;
	obj.GetObject = BaseObject_GetObject;
	obj.LoadImg = BaseObject_LoadImg;
	obj.Render = BaseObject_Render;
	obj.Free = BaseObject_Free;
	return obj;
}

void BaseObject_Destroy(BaseObject* obj)
{
	if (obj)
	{
		BaseObject_Free(obj);
	}
}


void BaseObject_SetRect(BaseObject* obj, const int x, const int y)
{
	obj->rect_.x = x;
	obj->rect_.y = y;
}

SDL_Rect BaseObject_GetRect(const BaseObject* obj)
{
	return obj->rect_;
}

SDL_Texture* BaseObject_GetObject(const BaseObject* obj)
{
	return obj->p_object_;
}

bool BaseObject_LoadImg(BaseObject* obj, const char* path, SDL_Renderer* screen)
{
	BaseObject_Free(obj);
	SDL_Texture* new_texture = NULL;

	SDL_Surface* load_surface = IMG_Load(path);
	if (load_surface != NULL)
	{
		SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
		new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
		if (new_texture != NULL)
		{
			obj->rect_.w = load_surface->w;
			obj->rect_.h = load_surface->h;
		}

		SDL_FreeSurface(load_surface);
	}
	obj->p_object_ = new_texture;

	return obj->p_object_ != NULL;
}

void BaseObject_Render(BaseObject* obj, SDL_Renderer* des, const SDL_Rect* clip)
{
	SDL_Rect renderquad = { obj->rect_.x, obj->rect_.y, obj->rect_.w, obj->rect_.h };
	SDL_RenderCopy(des, obj->p_object_, clip, &renderquad);
}

void BaseObject_Free(BaseObject* obj) {
    if (obj->p_object_ != NULL) {
        //SDL_DestroyTexture(obj->p_object_);
        obj->p_object_ = NULL;
        obj->rect_.h = 0;
        obj->rect_.w = 0;
    }
}