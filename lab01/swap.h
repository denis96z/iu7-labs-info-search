#ifndef LAB01_SWAP_H
#define LAB01_SWAP_H

#include <stdlib.h>
#include <string.h>

typedef void
(*swap_func_t)(void *ctx, void *a, void *b);

#define SWAP_FUNC(name) \
    void name(void *ctx, void *a, void *b)

#define tswap(t, a, b) \
    ({ t x = *(a); *(a) = *(b); *(b) = x; })

#define mswap(a, b, size) \
    ({                            \
        char x[(size)];           \
        memcpy(x, (a), (size));   \
        memcpy((a), (b), (size)); \
        memcpy((b), x, (size));   \
    })

#endif
