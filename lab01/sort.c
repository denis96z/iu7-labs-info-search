#include "sort.h"

#include "tree.h"

#include <assert.h>
#include <stdint.h>

INLINE void *
__arrp__(void *arr, size_t idx, size_t size)
{
    return arr + (idx * size);
}

#define arrp(idx) \
    __arrp__(arr, (idx), size)

SORT_FUNC(bubble_sort)
{
    for (size_t i = 0; i < (n - 2); ++i)
    {
        uint8_t sf = 0;

        for (size_t j = 0; j < (n - i - 1); ++j)
        {
            void *lp = arrp(j);
            void *rp = lp + size;

            if (cmp_func(ctx, lp, rp) > 0)
            {
                sf = 1;
                swap_func(ctx, lp, rp);
            }
        }

        if (!sf)
            break;
    }
}

SORT_FUNC(insertion_sort)
{
    for (size_t i = 1; i < n; ++i)
    {
	    for (ssize_t j = (ssize_t)i; j > 0; --j)
        {
            if (cmp_func(ctx, arrp(j - 1), arrp(j)) <= 0)
                break;
			swap_func(ctx, arrp(j - 1), arrp(j));
        }
    }
}

typedef struct
{
    void *ap;
    void *ctx;
    swap_func_t swap_func;
}
tree_sort_ctx_t;

TREE_TRAVERSE_FUNC(tree_sort_traverse_func)
{
    assert(ctx);
    tree_sort_ctx_t *sctx = ctx;

    uint8_t buff[size];
    memcpy(buff, item, size);

    sctx->swap_func(sctx->ctx, sctx->ap, buff);
    sctx->ap += size;
}

SORT_FUNC(tree_sort)
{
    tree_t t;
    tree_init(&t);

    for (size_t i = 0; i < n; ++i)
        tree_insert(ctx, &t, arrp(i), size, cmp_func);

    tree_sort_ctx_t sctx =
    {
        .ap = arr,
        .ctx = ctx,
        .swap_func = swap_func,
    };
    tree_in_order_traverse(&sctx, &t, tree_sort_traverse_func);

    tree_drop(&t);
}

INLINE ssize_t
quick_sort_partition(void *ctx,
                     void *arr, size_t size, ssize_t lidx, ssize_t ridx,
                     cmp_func_t cmp_func, swap_func_t swap_func)
{
    void *xp = arrp(ridx);

    ssize_t i = lidx - 1;
    for (ssize_t j = lidx; j < ridx; ++j)
    {
        if (cmp_func(ctx, arrp(j), xp) <= 0)
            swap_func(ctx, arrp(++i), arrp(j));
    }

    swap_func(ctx, arrp(i + 1), arrp(ridx));
    return i + 1;
}

static void
quick_sort_impl(void *ctx,
                void *arr, size_t size, ssize_t lidx, ssize_t ridx,
                cmp_func_t cmp_func, swap_func_t swap_func)
{
    if (lidx >= ridx)
        return;

    ssize_t idx = quick_sort_partition(ctx, arr, size, lidx, ridx, cmp_func, swap_func);
    quick_sort_impl(ctx, arr, size, lidx, idx - 1, cmp_func, swap_func);
    quick_sort_impl(ctx, arr, size, idx + 1, ridx, cmp_func, swap_func);
}

SORT_FUNC(quick_sort)
{
    quick_sort_impl(ctx, arr, size, (ssize_t)0, (ssize_t)(n - 1), cmp_func, swap_func);
}

ORDER_FUNC(shuffle)
{
    size_t ri = rand() % n;
    swap_func(NULL, arr, arrp(ri));

    for (size_t i = n - 2; i > 1; --i)
    {
        ri = rand() % (i + 1);
        swap_func(NULL, arrp(i), arrp(ri));
    }
}

ORDER_FUNC(make_sorted)
{
    bubble_sort(NULL, arr, n, size, cmp_func, swap_func);
}

static
CMP_FUNC(rcmp)
{
    assert(ctx);
    return -((cmp_func_t)ctx)(NULL, a, b);
}

ORDER_FUNC(make_rsorted)
{
    bubble_sort(cmp_func, arr, n, size, rcmp, swap_func);
}
