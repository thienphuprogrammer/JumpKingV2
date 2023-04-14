#ifndef LAYER_H_
#define LAYER_H_

#include "BaseObject.h"
#include "TileLayer.h"

typedef struct _Layer Layer;
struct _Layer {
    BaseObject base;
    void (*Render)(Layer*);
    void (*Update)(Layer*);
};

Layer* Layer_create();
void Layer_free(Layer* layer);

#endif // !LAYER_H_
