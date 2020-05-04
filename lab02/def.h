#ifndef LAB02_DEF_H
#define LAB02_DEF_H

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define auto __auto_type

#define likely(x)   __builtin_expect((long)(x), 1)
#define unlikely(x) __builtin_expect((long)(x), 0)

#define INLINE __attribute__((always_inline)) inline

#ifdef __FILE_PREFIX__
#define __FILE_REL__ \
    (__FILE__ + (sizeof(__FILE_PREFIX__) - 1))
#else
#define __FILE_REL__ __FILE__
#endif

#define min(x, y) \
    ({                       \
        auto xv = (x);       \
        auto yv = (y);       \
        (xv < yv) ? xv : yv; \
    })

#define max(x, y) \
    ({                       \
        auto xv = (x);       \
        auto yv = (y);       \
        (xv > yv) ? xv : yv; \
    })

#endif
