#ifndef LAB01_CMP_H
#define LAB01_CMP_H

#include <stdlib.h>

typedef ssize_t
(*cmp_func_t)(void *ctx, const void *a, const void *b);

#define CMP_FUNC(name) \
    ssize_t name(void *ctx, const void *a, const void *b)

#define __eq_cmp__(a, b) \
    (((a) == (b)) ? 0 : ((a) > (b)) ? 1 : -1)

#define tcmp(t, a, b) \
    __eq_cmp__(*((const t *)(a)), *((const t *)(b)))

#endif
