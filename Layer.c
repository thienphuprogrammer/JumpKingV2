#include "Layer.h"
#include <stdlib.h>

Layer* Layer_create(void) {
    Layer* layer = (Layer*)malloc(sizeof(Layer));
    layer->Render = TileLayer_Render;
    layer->Update = TileLayer_Update;
    return layer;
}

void Layer_free(Layer* layer) {
    free(layer);
}