#ifndef ROGUELIKE_TRIANGULATION_H
#define ROGUELIKE_TRIANGULATION_H

#include "../utils/list.h"

typedef struct vertex {
    int v_id;
    int vn_id;
    int vt_id;
} vertex_t;

void triangulate(list_t* face);

#endif //ROGUELIKE_TRIANGULATION_H
