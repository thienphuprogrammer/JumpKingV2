#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include "BaseObject.h"
#include "CommonFunction.h"

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8
#define PLAYER_MAX_JUMP_VAL 19
#define PLAYER_MIN_JUMP_VAL 10

typedef struct sMainObject MainObject;
struct sMainObject {
	BaseObject base_object;

	void (*Destroy)(MainObject*);
	bool (*LoadImg)(MainObject*, const char*, SDL_Renderer*);
	void (*Show)(MainObject*, SDL_Renderer* des);
	void (*HandleInputAction)(MainObject*, SDL_Event, SDL_Renderer*);
	void (*set_clips)(MainObject*);
	void (*DoPlayer)(MainObject*, Map*);
	void (*CheckToMap)(MainObject*, Map*);
	void (*SetMapXY)(MainObject*, const int, const int);
	void (*CenterEntityOnMap)(MainObject*, Map*);
	void (*UpdateImagePlayer)(MainObject*, SDL_Renderer*);

	int money_count;
	//BulletObject** p_bullet_list_;

	float x_val_;
	float y_val_;

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[8];

	Input input_type_;
	int frame_;
	int status_;
	bool on_ground_;

	int map_x_;
	int map_y_;

	int come_back_time_;

	int present_bullet_;

	float jump_val_;
};

enum WalkType {
	WALK_NONE = 1,
	WALK_RIGHT = 2,
	WALK_LEFT = 3,
	JUMP_UP = 4,
	JUMP_LEFT = 5,
	JUMP_RIGHT = 6,
	FALL_DOWN = 7,
	FALL_LEFT = 8,
	FALL_RIGHT = 9
};

MainObject MainObject_Create();
void MainObject_Destroy(MainObject* obj);
bool MainObject_LoadImg(MainObject* obj, const char* path, SDL_Renderer* screen);
void MainObject_Show(MainObject* obj, SDL_Renderer* des);
void MainObject_HandleInputAction(MainObject* obj, SDL_Event events, SDL_Renderer* screen);
void MainObject_SetClips(MainObject* obj);
void MainObject_DoPlayer(MainObject* obj, Map* map_data);
void MainObject_CheckToMap(MainObject* obj, Map* map_data);
void MainObject_SetMapXY(MainObject* obj, const int map_x, const int map_y);
void MainObject_CenterEntityOnMap(MainObject* obj, Map* map_data);
void MainObject_UpdateImagePlayer(MainObject* obj, SDL_Renderer* des);
//void MainObject_SetBulletList(MainObject* obj, BulletObject** bullet_list);
//BulletObject** MainObject_GetBulletList(MainObject* obj);
void MainObject_HandldeBulle(MainObject* obj, SDL_Renderer* des);
void MainObject_IncreaseMoney(MainObject* obj);

#endif // !MAIN_OBJECT_H_
