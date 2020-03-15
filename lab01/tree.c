#include "tree.h"

#include <string.h>

void
tree_init(tree_t *tree)
{
    tree->root = NULL;
}

void
tree_drop(tree_t *tree)
{
    tree_clear(tree);
}

void
tree_insert(void *ctx, tree_t *tree, void *item, size_t size, cmp_func_t cmp_func)
{
    tree_node_t *nn = malloc(sizeof(tree_node_t));

    nn->data = malloc(size);
    memcpy(nn->data, item, size);

    nn->data_size = size;
    nn->lnode = NULL;
    nn->rnode = NULL;

    tree_node_t *t = NULL;
    tree_node_t *n = tree->root;
    while (n)
    {
        t = n;
        if (cmp_func(ctx, nn->data, n->data) < 0)
            n = n->lnode;
        else
            n = n->rnode;
    }

    if (!t)
        tree->root = nn;
    else if (cmp_func(ctx, nn->data, t->data) < 0)
        t->lnode = nn;
    else
        t->rnode = nn;
}

static void
tree_clear_impl(tree_node_t *node)
{
    if (!node)
        return;

    tree_node_t *lnode = node->lnode;
    tree_node_t *rnode = node->rnode;

    free(node->data);
    free(node);

    tree_clear_impl(lnode);
    tree_clear_impl(rnode);
}

void
tree_clear(tree_t *tree)
{
    tree_clear_impl(tree->root);
    tree->root = NULL;
}

static void
tree_in_order_traverse_impl(void *ctx, const tree_node_t *node, tree_traverse_func_t tfunc)
{
    if (node->lnode)
        tree_in_order_traverse_impl(ctx, node->lnode, tfunc);

    tfunc(ctx, node->data, node->data_size);

    if (node->rnode)
        tree_in_order_traverse_impl(ctx, node->rnode, tfunc);
}

void
tree_in_order_traverse(void *ctx, const tree_t *tree, tree_traverse_func_t tfunc)
{
    if (!tree->root)
        return;

    tree_in_order_traverse_impl(ctx, tree->root, tfunc);
}
