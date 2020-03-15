#include "task.h"

#include <assert.h>

typedef struct
{
    size_t ncmp;
    size_t nswap;
}
int_cmp_swap_ctx_t;

#define ctx_inc_stat(ctx, stat) \
    ({ ++(((int_cmp_swap_ctx_t *)(ctx))->stat); })

CMP_FUNC(int_cmp)
{
    return tcmp(int_t, a, b);
}

SWAP_FUNC(int_swap)
{
    tswap(int_t, (int_t *)a, (int_t *)b);
}

CMP_FUNC(int_ctx_cmp)
{
    assert(ctx);
    ctx_inc_stat(ctx, ncmp);

    return int_cmp(ctx, a, b);
}

SWAP_FUNC(int_ctx_swap)
{
    assert(ctx);
    ctx_inc_stat(ctx, nswap);

    return int_swap(ctx, a, b);
}

int_arr_t *
init_int_arr(int_arr_t *arr, size_t n)
{
    int_t *x = calloc(n, sizeof(int_t));
    if (!x)
        return NULL;

    arr->n = n;
    arr->items = x;

    return arr;
}

int_arr_t *
init_int_arr_range(int_arr_t *arr, size_t n,
                   int_t min_value, int_t max_value)
{
    int_t *x = malloc(n * sizeof(int_t));
    if (!x)
        return NULL;

    for (size_t i = 0; i < n; ++i)
        x[i] = min_value + (rand() % (max_value - min_value));

    arr->n = n;
    arr->items = x;

    return arr;
}

void
drop_int_arr(int_arr_t *arr)
{
    free(arr->items);
    memset(arr, 0, sizeof(int_arr_t));
}

void
order_int_arr(int_arr_t *arr, order_func_t order_func)
{
    order_func(arr->items, arr->n, sizeof(int_t), int_cmp, int_swap);
}

int_arr_sort_stat_t
sort_int_arr(int_arr_t *arr, sort_func_t sort_func)
{
    int_cmp_swap_ctx_t ctx = { 0 };
    sort_func(&ctx, arr->items, arr->n, sizeof(int_t), int_ctx_cmp, int_ctx_swap);

    int_arr_sort_stat_t stat =
    {
            .ncmp  = ctx.ncmp,
            .nswap = ctx.nswap,
    };

    return stat;
}
