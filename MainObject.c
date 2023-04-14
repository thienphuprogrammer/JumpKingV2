#include "MainObject.h"
#include <math.h>

#define LOCA_START 170 * 32

MainObject MainObject_Create() {

	MainObject obj;
	obj.base_object = *BaseObject_Create();
	obj.frame_ = 0;
	obj.x_pos_ = 0;
	obj.y_pos_ = LOCA_START;
	obj.x_val_ = 0;
	obj.y_val_ = 0;
	obj.width_frame_ = 0;
	obj.height_frame_ = 0;
	obj.status_ = WALK_NONE;

	obj.input_type_.right_ = 0;
	obj.input_type_.left_ = 0;
	obj.input_type_.jump_ = 0;
	obj.input_type_.down_ = 0;
	obj.input_type_.up_ = 0;

	obj.ready_jump_ = false;
	obj.running = false;
	obj.jump_left = false;
	obj.jump_right = false;

	obj.check_collion_wall = 0;
	obj.collion_wall = 1;

	obj.on_ground_ = 0; // giá trị phù hợp với kiểu bool
	obj.map_x_ = 0;
	obj.map_y_ = 0;
	obj.come_back_time_ = 0;
	obj.money_count = 0;
	obj.jump_val_ = 0; // giá trị phù hợp với kiểu int
	obj.val_respond_collison = 0;

	obj.Destroy = MainObject_Destroy;
	obj.LoadImg = MainObject_LoadImg;
	obj.Show = MainObject_Show;
	obj.HandleInputAction = MainObject_HandleInputAction;
	obj.set_clips = MainObject_SetClips;
	obj.DoPlayer = MainObject_DoPlayer;
	obj.CheckToMap = MainObject_CheckToMap;
	obj.SetMapXY = MainObject_SetMapXY;
	obj.CenterEntityOnMap = MainObject_CenterEntityOnMap;
	obj.UpdateImagePlayer = MainObject_UpdateImagePlayer;
	return obj;
}

void MainObject_Destroy(MainObject* obj) {
	if (obj != NULL) {
		obj->base_object.Free(&obj->base_object);
		obj->base_object.Destroy(&obj->base_object);
	}
}

void MainObject_SetMapXY(MainObject* obj, const int map_x, const int map_y)
{
	obj->map_x_ = map_x;
	obj->map_y_ = map_y;
}

bool MainObject_LoadImg(MainObject* obj, const char* path, SDL_Renderer* screen) {
    bool ret = obj->base_object.LoadImg(&(obj->base_object), path, screen);
    obj->base_object.rect_.x;


    if (ret == true) {
        obj->width_frame_ = obj->base_object.rect_.w / NumPrite;
        obj->height_frame_ = obj->base_object.rect_.h;
    }
    return ret;
}

void MainObject_SetClips(MainObject* obj)
{
	if (obj->width_frame_ > 0 && obj->height_frame_ > 0)
	{
		for (int i = 0; i < NumPrite; i++)
		{
			obj->frame_clip_[i].x = i * obj->width_frame_;
			obj->frame_clip_[i].y = 0;
			obj->frame_clip_[i].w = obj->width_frame_;
			obj->frame_clip_[i].h = obj->height_frame_;
		}
	}
}

void MainObject_Show(MainObject* obj, SDL_Renderer* des) {
	obj->UpdateImagePlayer(obj, des);
	if (obj->input_type_.left_ == 1 ||
		obj->input_type_.right_ == 1) {
		if (!obj->ready_jump_ && obj->on_ground_ && !obj->jump_left && !obj->jump_right)
		{
			obj->frame_++;
		}
	}
	else {
		obj->frame_ = 0;
	}

	if (obj->frame_ >= NumPrite) {
		obj->frame_ = 0;
	}

	if (obj->come_back_time_ == 0) {
		obj->base_object.rect_.x = obj->x_pos_ - obj->map_x_;
		obj->base_object.rect_.y = obj->y_pos_ - obj->map_y_;

		SDL_Rect* current_clip = &obj->frame_clip_[obj->frame_];

		SDL_Rect renderQuad = { obj->base_object.rect_.x, obj->base_object.rect_.y , 
			obj->width_frame_, obj->height_frame_ };

		SDL_RenderCopy(des, obj->base_object.p_object_, current_clip, &renderQuad);
	}
}

void MainObject_HandleInputAction(MainObject* obj, SDL_Event events, SDL_Renderer* screen) {
	if (events.type == SDL_KEYDOWN) {
		switch (events.key.keysym.sym)
		{
		case SDLK_SPACE:
			if(!obj->ready_jump_) obj->jump_val_ = PLAYER_MIN_JUMP_VAL;
			obj->ready_jump_ = true;
			break;
		case SDLK_RIGHT:
			obj->input_type_.right_ = 1;
			obj->input_type_.left_ = 0;
			obj->running = true;
			if (!obj->ready_jump_) obj->status_ = WALK_RIGHT;
			break;
		case SDLK_LEFT:
			obj->input_type_.left_ = 1;
			obj->input_type_.right_ = 0;
			obj->running = true;
			if (!obj->ready_jump_) obj->status_ = WALK_LEFT;
			break;
		default:
			break;
		}
	}
	else if (events.type == SDL_KEYUP) {
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:			
			obj->running = false;
			obj->input_type_.right_ = 0;	
			break;
		case SDLK_LEFT:
			obj->running = false;
			obj->input_type_.left_ = 0;
			break;
		case SDLK_SPACE:
			obj->input_type_.jump_ = 1;
			obj->ready_jump_ = false;

			if (obj->input_type_.left_ == 1) {
				obj->jump_left = true;
			}
			else if (obj->input_type_.right_ == 1) {
				obj->jump_right = true;
			}
			obj->input_type_.right_ = 0;
			obj->input_type_.left_ = 0;
			break;
		default:
			break;
		}
	}
}

void MainObject_DoPlayer(MainObject* obj, Map* map_data) {
	if (obj->come_back_time_ == 0) {
		obj->x_val_ = 0;
		obj->y_val_ += (GRAVITY_SPEED);

		//Hanlde Set Value Jump 
		if (obj->ready_jump_)
		{
			obj->jump_val_ += 0.4;
			if (obj->jump_val_ >= PLAYER_MAX_JUMP_VAL) {
				obj->jump_val_ = PLAYER_MAX_JUMP_VAL;
			}
		}

		//Chechk Max Fall Speedc
		if (obj->y_val_ >= MAX_FALL_SPEED) {
			obj->y_val_ = MAX_FALL_SPEED;
		}
		if (obj->on_ground_)
		{
			if (obj->input_type_.left_ == 1 && !obj->ready_jump_) {
				obj->x_val_ -= PLAYER_SPEED;
			}
			else if (obj->input_type_.right_ == 1 && !obj->ready_jump_) {
				obj->x_val_ += PLAYER_SPEED;
			}
		}

		//Handle jumping
		if (obj->input_type_.jump_ == 1) {
			if (obj->on_ground_ == true) {
				obj->on_ground_ = false;
				obj->y_val_ = -obj->jump_val_; 
				obj->jump_val_ = 0;
				//obj->jump_val_ = PLAYER_MIN_JUMP_VAL;
			}
			obj->input_type_.jump_ = 0;
		}

		// Handle jumping left or right
		if ((obj->jump_left || obj->jump_right) && !obj->on_ground_) {
			if (obj->jump_left) {
				obj->x_val_ -= PLAYER_SPEED * obj->collion_wall;
				
				if (obj->check_collion_wall == 1) {
					obj->val_respond_collison += 0.2;
					obj->x_val_ += (obj->val_respond_collison) * obj->collion_wall;
					if (obj->val_respond_collison >= PLAYER_SPEED) {
						obj->val_respond_collison = PLAYER_SPEED;
					}
				}
			}
			else if (obj->jump_right) {
				obj->x_val_ += PLAYER_SPEED * obj->collion_wall;
				
				if (obj->check_collion_wall == 1) {
					obj->val_respond_collison += 0.2;
					obj->x_val_ -= (obj->val_respond_collison) * obj->collion_wall;
					if (obj->val_respond_collison >= PLAYER_SPEED) {
						obj->val_respond_collison = PLAYER_SPEED;
					}
				}
			}
		}
		else {
			obj->jump_left = false;
			obj->jump_right = false;
		}
		
		obj->CheckToMap(obj, map_data);
		obj->CenterEntityOnMap(obj, map_data);
	}
	if (obj->come_back_time_ > 0) {
		obj->come_back_time_--;
		if (obj->come_back_time_ == 0) {
			obj->on_ground_ = false;
			if (obj->x_pos_ > 256) {
				obj->x_pos_ -= 256;
			}
			else {
				obj->x_pos_ = 0;
			}
			obj->y_pos_ = LOCA_START;
			obj->x_val_ = 0;
		}
	}
}

void MainObject_CenterEntityOnMap(MainObject* obj, Map* map_data) {
	map_data->start_x_ = obj->x_pos_ - (SCREEN_WIDTH / 2);
	if (map_data->start_x_ < 0) {
		map_data->start_x_ = 0;
	}
	else if (map_data->start_x_ + SCREEN_WIDTH >= map_data->max_x_) {
		map_data->start_x_ = map_data->max_x_ - SCREEN_WIDTH;
	}

	map_data->start_y_ = obj->y_pos_ - (SCREEN_HEIGHT / 2);
	if (map_data->start_y_ < 0) {
		map_data->start_y_ = 0;
	}
	else if (map_data->start_y_ + SCREEN_HEIGHT >= map_data->max_y_) {
		map_data->start_y_ = map_data->max_y_ - SCREEN_HEIGHT;
	}
}

void MainObject_CheckToMap(MainObject* obj, Map* map_data) {
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	//Check horizontial 
	int height_min = obj->height_frame_ < TILE_SIZE ? obj->height_frame_ : TILE_SIZE;

	x1 = (obj->x_pos_ + obj->x_val_) / TILE_SIZE;
	x2 = (obj->x_pos_ + obj->x_val_ + obj->width_frame_ - 1) / TILE_SIZE;

	y1 = (obj->y_pos_) / TILE_SIZE;
	y2 = (obj->y_pos_ + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 <= MAX_MAP_Y) {
		if (obj->x_val_ > 0) {
			int val1 = map_data->tile[y1][x2];
			int val2 = map_data->tile[y2][x2];

			if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
				obj->x_pos_ = x2 * TILE_SIZE;
				obj->x_pos_ -= obj->width_frame_ + 1;
				obj->x_val_ = 0;
				obj->collion_wall *= -1;
				obj->check_collion_wall = 1;
			}
		}
		else if (obj->x_val_ < 0) {
			int val1 = map_data->tile[y1][x1];
			int val2 = map_data->tile[y2][x1];

			if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
				obj->x_pos_ = (x1 + 1) * TILE_SIZE;
				obj->x_val_ = 0;
				obj->collion_wall *= -1;
				obj->check_collion_wall = 1;
			}
		}
	}

	//Check vartical
	int width_min = obj->width_frame_ < TILE_SIZE ? obj->width_frame_ : TILE_SIZE;
	x1 = (obj->x_pos_) / TILE_SIZE;
	x2 = (obj->x_pos_ + width_min) / TILE_SIZE;

	y1 = (obj->y_pos_ + obj->y_val_) / TILE_SIZE;
	y2 = (obj->y_pos_ + obj->y_val_ + obj->height_frame_ - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (obj->y_val_ > 0) {
			int val1 = map_data->tile[y2][x1];
			int val2 = map_data->tile[y2][x2];

			if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
				obj->y_pos_ = y2 * TILE_SIZE;
				obj->y_pos_ -= obj->height_frame_ - 3;
				obj->y_val_ = 0;
				obj->on_ground_ = true;
				obj->collion_wall = 1;
				obj->check_collion_wall = 0;
				obj->val_respond_collison = 0;

				if (obj->status_ == WALK_NONE) {
					obj->status_ = WALK_RIGHT;
				}
			}
			else {
				obj->on_ground_ = false;
			}
		}
		else if (obj->y_val_ < 0) {
			int val1 = map_data->tile[y1][x1];
			int val2 = map_data->tile[y1][x2];

			if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
				obj->y_pos_ = (y1 + 1) * TILE_SIZE;
				obj->y_val_ = 0;
			}
		}
	}

	obj->x_pos_ += obj->x_val_;
	obj->y_pos_ += obj->y_val_;

	if (obj->x_pos_ < 0) {
		obj->x_pos_ = 0;
		obj->collion_wall *= -1;
		obj->check_collion_wall = 1;
	}
	else if (obj->x_pos_ + obj->width_frame_ > map_data->max_x_) {
		obj->x_pos_ = map_data->max_x_ - obj->width_frame_ - 1;
		obj->collion_wall *= -1;
		obj->check_collion_wall = 1;
	}

	if (obj->y_pos_ > map_data->max_y_) {
		obj->come_back_time_ = 60;
	}
}

void MainObject_UpdateImagePlayer(MainObject* obj, SDL_Renderer* des) {
	if (obj->on_ground_) {
		if (obj->status_ == WALK_LEFT) {
			obj->base_object.LoadImg(&obj->base_object, "img/MainObject/MoveLeft.png", des);
		}
		else {
			obj->base_object.LoadImg(&obj->base_object, "img/MainObject/MoveRight.png", des);
		}
	}
	else {
		if (obj->status_ == WALK_LEFT) {
			obj->base_object.LoadImg(&obj->base_object, "img/MainObject/JumpLeft.png", des);
		}
		else {
			obj->base_object.LoadImg(&obj->base_object, "img/MainObject/JumpRight.png", des);
		}
	}
}