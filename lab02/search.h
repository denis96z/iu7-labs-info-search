#ifndef LAB02_SEARCH_H
#define LAB02_SEARCH_H

#include "def.h"

#define SEARCH_FUNC(name) \
    ssize_t name(const char *s, ssize_t slen,   \
                 const char *ss, ssize_t sslen)

typedef SEARCH_FUNC((*search_func_t));

SEARCH_FUNC(naive_search);
SEARCH_FUNC(rabin_karp_search);
SEARCH_FUNC(fsm_search);
SEARCH_FUNC(kmp_search);
SEARCH_FUNC(boyer_moore_search);

#endif
