#ifndef ROGUELIKE_RELATIONSHIP_H
#define ROGUELIKE_RELATIONSHIP_H

typedef struct {
    size_t entity_id;
    size_t first_child;
    size_t prev;
    size_t next;
    size_t parent;
} relationship_t;

#endif //ROGUELIKE_RELATIONSHIP_H
