#ifndef LAB01_TREE_H
#define LAB01_TREE_H

#include "def.h"
#include "cmp.h"

typedef struct tree_node
{
    void *data;
    size_t data_size;

    struct tree_node *lnode;
    struct tree_node *rnode;
}
tree_node_t;

typedef struct
{
    struct tree_node *root;
}
tree_t;

void
tree_init(tree_t *tree);

void
tree_drop(tree_t *tree);

void
tree_insert(void *ctx, tree_t *tree, void *item, size_t size, cmp_func_t cmp_func);

void
tree_clear(tree_t *tree);

#define TREE_TRAVERSE_FUNC(name) \
    void name(void *ctx, const void *item, size_t size)

typedef TREE_TRAVERSE_FUNC((*tree_traverse_func_t));

void
tree_in_order_traverse(void *ctx, const tree_t *tree, tree_traverse_func_t tfunc);

#endif
