#define _CRT_SECURE_NO_WARNINGS

#include "TileLayer.h"
#include "CommonFunction.h"

TileLayer* TileLayer_new(int tilesize, int rowcount, int colcount, TileMap* tilemap, TilesetList* tilesets) {
	TileLayer* layer = (TileLayer*)malloc(sizeof(TileLayer));
	layer->Update = TileLayer_Update;
	layer->Render = TileLayer_Render;
	layer->GetTileMap = TileLayer_GetTileMap;
	layer->m_TileSize = tilesize;
	layer->m_RowCount = rowcount;
	layer->m_ColCount = colcount;
	layer->m_Tilemap = tilemap;

	layer->texture_map = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);

	// //Avoid unnecessary memory allocations by reusing the existing GPtrArray
	layer->m_Tilesets = tilesets; // increment the reference count of tilesets

	// Load textures asynchronously in background thread or coroutine
	for (GList* iter = layer->m_Tilesets; iter != NULL; iter = iter->next) {
		Tileset* tileset = (Tileset*)iter->data;
		char* file_path = g_strdup_printf("assets/maps/%s", tileset->Source);

		BaseObject* base = BaseObject_Create();
		base->LoadImg(base, file_path, g_screen);

		GQuark name = g_quark_from_string(tileset->Name);

		g_hash_table_insert(layer->texture_map, (gpointer)tileset->Name, base);

		g_free(file_path);
	}

	return layer;
}

void TileLayer_free(TileLayer* layer)
{
	if (layer == NULL) {
		return;
	}

	// Free the tilesets
	/*for (GList* iter = layer->m_Tilesets; iter != NULL; iter = g_list_next(iter)) {
		Tileset* tileset = iter->data;
		g_free(tileset);
	}*/

	//g_ptr_array_free(layer->m_Tilesets, TRUE);

	// Free the tilemap
	for (int i = 0; i < layer->m_Tilemap->len; i++) {
		GPtrArray* row = (GPtrArray*)g_ptr_array_index(layer->m_Tilemap, i);
		g_ptr_array_free(row, TRUE);
	}
	g_ptr_array_free(layer->m_Tilemap, TRUE);

	// Free the tile layer
	free(layer);
}

void TileLayer_Render(TileLayer* layer, int tileID, int i, int j)
{
	/*if (layer->m_Tilemap->len == 0) {
		return;
	}

	for (int i = 0; i < layer->m_RowCount; i++)
	{
		for (int j = 0; j < layer->m_ColCount; j++)
		{

			int tileID = ((int)g_ptr_array_index(layer->m_Tilemap, i * layer->m_ColCount + j));

			if (tileID == 0)
			{
				continue;
			} 
			else 
			{*/
				guint index = 0;
				gboolean tilesetFound = FALSE;
				if (layer->m_Tilemap->len > 1)
				{
					for (GList* k = layer->m_Tilesets; k != NULL; k = k->next)
					{
						Tileset* tileset = (Tileset*)k->data;
						if (tileID >= tileset->FirstID && tileID <= tileset->LastID)
						{
							tileID = tileID + tileset->TileCount - tileset->LastID;
							tilesetFound = TRUE;
							break;
						}
						index++;
					}
				}
				if (tilesetFound)
				{
					Tileset* ts = (Tileset*)g_list_nth_data(layer->m_Tilesets, index);
					int tileRow = tileID / ts->ColCount;
					int tileCol = tileID - tileRow * ts->ColCount - 1;

					if (tileID % ts->ColCount == 0)
					{
						tileRow--;
						tileCol = ts->ColCount - 1;
					}

					BaseObject* bass_texture = BaseObject_Create();
					bass_texture = (BaseObject*)g_hash_table_lookup(layer->texture_map, (gpointer)ts->Name);
					if (bass_texture)
					{
						//bass_texture->DrawTile(g_screen, bass_texture->GetObject(bass_texture), ts->TileSize, j * ts->TileSize, i * ts->TileSize, tileRow, tileCol, (SDL_RendererFlip)NULL);
						bass_texture->DrawTile(g_screen, bass_texture->GetObject(bass_texture), ts->TileSize, j, i, tileRow, tileCol, (SDL_RendererFlip)NULL);
					}
					//TextureManager::GetInstance()->DrawTile(ts.Name, ts.TIleSize, j * ts.TIleSize, i * ts.TIleSize, tileRow, tileCol);
					//SDL_RenderPresent(g_screen);
	//			}
	//		}

	//	}
	}
};

void TileLayer_Update(TileLayer* layer)
{

}

TileMap *TileLayer_GetTileMap(TileLayer* layer)
{
    return layer->m_Tilemap;
}