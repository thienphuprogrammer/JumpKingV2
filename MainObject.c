#include "MainObject.h"
 
#include "MainObject.h"

MainObject MainObject_Create() {

	MainObject obj;
	obj.base_object = BaseObject_Create();
	obj.frame_ = 0;
	obj.x_pos_ = 0;
	obj.y_pos_ = 0;
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
	obj.on_ground_ = false;
	obj.map_x_ = 0;
	obj.map_y_ = 0;
	obj.come_back_time_ = 0;
	//obj.present_bullet_ = BulletObject_SPHERE_BULLET;
	obj.money_count = 0;
	obj.jump_val_ = PLAYER_MIN_JUMP_VAL;

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
	if (obj) {
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
        obj->width_frame_ = obj->base_object.rect_.w / 8;
        obj->height_frame_ = obj->base_object.rect_.h;
    }
    return ret;
}

void MainObject_SetClips(MainObject* obj)
{
	if (obj->width_frame_ > 0 && obj->height_frame_ > 0)
	{
		for (int i = 0; i < 8; i++)
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
		obj->frame_++;
	}
	else {
		obj->frame_ = 0;
	}

	if (obj->frame_ >= 8) {
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
			obj->jump_val_ += 0.5;
			if (obj->jump_val_ >= PLAYER_MAX_JUMP_VAL)
			{
				obj->jump_val_ = PLAYER_MAX_JUMP_VAL;
			}
			break;
		case SDLK_RIGHT:
			obj->status_ = WALK_RIGHT;
			obj->input_type_.right_ = 1;
			obj->input_type_.left_ = 0;
			break;
		case SDLK_LEFT:
			obj->status_ = WALK_LEFT;
			obj->input_type_.left_ = 1;
			obj->input_type_.right_ = 0;
			break;
		
		case SDLK_1:
			//obj->present_bullet_ = BulletObject::SPHERE_BULLET;
			break;
		case SDLK_2:
			//present_bullet_ = BulletObject::LASER_BULLET;
			break;
		default:
			break;
		}
	}
	else if (events.type == SDL_KEYUP) {
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
			obj->input_type_.right_ = 0;
			break;
		case SDLK_LEFT:
			obj->input_type_.left_ = 0;
			break;
		case SDLK_SPACE:
			obj->input_type_.jump_ = 1;
			break;
		default:
			break;
		}
	}
	if (events.type == SDL_MOUSEBUTTONDOWN) {
		if (events.button.button == SDL_BUTTON_RIGHT) {
			obj->input_type_.jump_ = 1;
		}
		//else if (events.button.button == SDL_BUTTON_LEFT) {
		//	BulletObject* p_bullet = new BulletObject();
		//	p_bullet->set_bulet_type(present_bullet_);
		//	p_bullet->LoadImgBullet(screen);
		//	if (status_ == WALK_LEFT) {
		//		p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
		//		p_bullet->SetRect(this->rect_.x, this->rect_.y + height_frame_ * 0.25);
		//	}
		//	else if (status_ == WALK_RIGHT) {
		//		p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
		//		p_bullet->SetRect(this->rect_.x + width_frame_ - 20, this->rect_.y + height_frame_ * 0.3);
		//	}
		//	p_bullet->set_x_val(20);
		//	p_bullet->set_y_val(20);
		//	p_bullet->set_is_move(true);
		//	p_bullet_list_.push_back(p_bullet);
		//}
	}
}

//void MainObject_HandldeBulle(MainObject* obj, SDL_Renderer* des) {
//	for (size_t i = 0; i < obj->p_bullet_list_.size(); i++)
//	{
//		BulletObject* p_bullet = p_bullet_list_.at(i);
//		if (p_bullet != NULL) {
//			if (p_bullet->get_is_move() == true) {
//				p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
//				p_bullet->Render(des);
//			}
//			else {
//				p_bullet_list_.erase(p_bullet_list_.begin() + i);
//				if (p_bullet != NULL) {
//					delete p_bullet;
//					p_bullet = NULL;
//				}
//			}
//		}
//	}
//}

void MainObject_DoPlayer(MainObject* obj, Map* map_data) {
	if (obj->come_back_time_ == 0) {
		obj->x_val_ = 0;
		obj->y_val_ += (GRAVITY_SPEED);

		if (obj->y_val_ >= MAX_FALL_SPEED) {
			obj->y_val_ = MAX_FALL_SPEED;
		}

		if (obj->input_type_.left_ == 1) {
			obj->x_val_ -= PLAYER_SPEED;
		}
		else if (obj->input_type_.right_ == 1) {
			obj->x_val_ += PLAYER_SPEED;
		}

		if (obj->input_type_.jump_ == 1) {
			if (obj->on_ground_ == true) {
				obj->y_val_ = -obj->jump_val_;
				obj->on_ground_ = false;
				obj->jump_val_ = PLAYER_MIN_JUMP_VAL;
			}
			obj->input_type_.jump_ = 0;
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
			obj->y_pos_ = 0;
			obj->x_val_ = 0;
			obj->y_val_ = 0;
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

			if (val1 == STATE_MONEY || val2 == STATE_MONEY) {
				map_data->tile[y1][x2] = 0;
				map_data->tile[y2][x2] = 0;
				MainObject_IncreaseMoney(obj);
			}
			else {
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					obj->x_pos_ = x2 * TILE_SIZE;
					obj->x_pos_ -= obj->width_frame_ + 1;
					obj->x_val_ = 0;
				}
			}
		}
		else if (obj->x_val_ < 0) {
			int val1 = map_data->tile[y1][x1];
			int val2 = map_data->tile[y2][x1];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY) {
				map_data->tile[y1][x1] = 0;
				map_data->tile[y2][x1] = 0;
				MainObject_IncreaseMoney(obj);
			}

			else {
				if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
					obj->x_pos_ = (x1 + 1) * TILE_SIZE;
					obj->x_val_ = 0;
				}
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

			if (val1 == STATE_MONEY || val2 == STATE_MONEY) {
				map_data->tile[y2][x1] = 0;
				map_data->tile[y2][x2] = 0;
				MainObject_IncreaseMoney(obj);
			}
			else if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
				obj->y_pos_ = y2 * TILE_SIZE;
				obj->y_pos_ -= obj->height_frame_ + 1;
				obj->y_val_ = 0;
				obj->on_ground_ = true;

				if (obj->status_ == WALK_NONE) {
					obj->status_ = WALK_RIGHT;
				}
			}
		}
		else if (obj->y_val_ < 0) {
			int val1 = map_data->tile[y1][x1];
			int val2 = map_data->tile[y1][x2];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY) {
				map_data->tile[y1][x1] = 0;
				map_data->tile[y1][x2] = 0;
				MainObject_IncreaseMoney(obj);
			}
			else if (val1 != BLANK_TILE || val2 != BLANK_TILE) {
				obj->y_pos_ = (y1 + 1) * TILE_SIZE;
				obj->y_val_ = 0;
			}
		}
	}


	obj->x_pos_ += obj->x_val_;
	obj->y_pos_ += obj->y_val_;

	if (obj->x_pos_ < 0) {
		obj->x_pos_ = 0;
	}
	else if (obj->x_pos_ + obj->width_frame_ > map_data->max_x_) {
		obj->x_pos_ = map_data->max_x_ - obj->width_frame_ - 1;
	}

	if (obj->y_pos_ > map_data->max_y_) {
		obj->come_back_time_ = 60;
	}
}

void MainObject_IncreaseMoney(MainObject* obj) {
	obj->money_count++;
}

void MainObject_UpdateImagePlayer(MainObject* obj, SDL_Renderer* des) {
	if (obj->on_ground_) {
		if (obj->status_ == WALK_LEFT) {
			obj->base_object.LoadImg(&obj->base_object, "img/player sprite/player_left.png", des);
		}
		else {
			obj->base_object.LoadImg(&obj->base_object, "img/player sprite/player_right.png", des);
		}
	}
	else {
		if (obj->status_ == WALK_LEFT) {
			obj->base_object.LoadImg(&obj->base_object, "img/player sprite/jum_left.png", des);
		}
		else {
			obj->base_object.LoadImg(&obj->base_object, "img/player sprite/jum_right.png", des);
		}
	}
}
