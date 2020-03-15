#ifndef LAB01_SORT_H
#define LAB01_SORT_H

#include <stdlib.h>

#include "def.h"
#include "cmp.h"
#include "swap.h"

#define __sort_func_args__ \
    void *ctx, void *arr, size_t n, size_t size, \
        cmp_func_t cmp_func, swap_func_t swap_func

#define SORT_FUNC(name) \
    void name(__sort_func_args__)

typedef SORT_FUNC((*sort_func_t));

SORT_FUNC(bubble_sort);
SORT_FUNC(insertion_sort);
SORT_FUNC(tree_sort);
SORT_FUNC(quick_sort);

#define ORDER_FUNC(name) \
    void name(void *arr, size_t n, size_t size, \
              cmp_func_t cmp_func, swap_func_t swap_func)

typedef ORDER_FUNC((*order_func_t));

ORDER_FUNC(shuffle);
ORDER_FUNC(make_sorted);
ORDER_FUNC(make_rsorted);

#endif
