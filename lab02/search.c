#include "search.h"

INLINE ssize_t
xstrlen(const char *s, ssize_t slen)
{
    return (slen == -1) ? (likely(s) ? strlen(s) : 0) : slen;
}

INLINE bool
xstrneq(const char *s1, const char *s2, ssize_t n)
{
    return (n == -1) ? !strcmp(s1, s2) : !strncmp(s1, s2, (size_t)n);
}

INLINE bool
xissuffix(const char *s, ssize_t sn, const char *p, ssize_t pn)
{
    sn = xstrlen(s, sn);
    pn = xstrlen(p, pn);

    if (!pn)
        return true;
    if (sn < pn)
        return false;

    return xstrneq(s + sn - pn, p, pn);
}

#define search_prepare_s_ss_len() \
    ({                              \
        slen = xstrlen(s, slen);    \
        sslen = xstrlen(ss, sslen); \
        if (sslen > slen)           \
            return -1;              \
        if (unlikely(sslen == 0))   \
            return 0;               \
    })

SEARCH_FUNC(naive_search)
{
    search_prepare_s_ss_len();

    for (size_t i = 0; i <= (slen - sslen); ++i)
        if (xstrneq(s + i, ss, sslen))
            return i;

    return -1;
}

#define RABIN_KARP_ASIZE        10
#define RABIN_KARP_MAGIC_NUMBER 13

INLINE ssize_t
rabin_karp_hash(const char *s, ssize_t n)
{
    assert(n > 0);

    ssize_t h = 0;
    for (ssize_t i = 0; i < n; ++i)
        h += s[i] + (RABIN_KARP_ASIZE * h);

    return h % RABIN_KARP_MAGIC_NUMBER;
}

SEARCH_FUNC(rabin_karp_search)
{
    search_prepare_s_ss_len();

    ssize_t sh = rabin_karp_hash(s, sslen);
    ssize_t ssh = rabin_karp_hash(ss, sslen);

    for (ssize_t i = 0; i <= (slen - sslen); ++i)
    {
        if ((sh == ssh) && xstrneq(ss, s + i, sslen))
            return i;
        sh = rabin_karp_hash(s + i + 1, sslen);
    }

    return -1;
}

#define ASIZE 256

typedef struct
{
    size_t n;
    size_t q;
    size_t **t;
}
suffix_fsm_t;

INLINE void
suffix_fsm_init(suffix_fsm_t *fsm, const char *s, ssize_t slen)
{
    slen = xstrlen(s, slen);
    size_t n = (size_t)slen + 1;

    fsm->q = 0;
    fsm->n = xstrlen(s, n);

    fsm->t = malloc(n * sizeof(size_t *));
    for (size_t i = 0; i < n; ++i)
        fsm->t[i] = calloc(ASIZE, sizeof(size_t));

    char *sb = malloc(n * sizeof(char));
    for (ssize_t i = 0; i < n; ++i)
    {
        sb[max(i - 1, 0)] = s[i - 1];

        for (size_t j = 1; j < ASIZE; ++j)
        {
            sb[i] = (char)j;

            ssize_t k = min(n, i + 2);
            while(!xissuffix(sb, i + 1, s, --k));

            fsm->t[i][j] = (size_t)k;
        }
    }

    free(sb);
}

INLINE void
suffix_fsm_drop(suffix_fsm_t *fsm)
{
    for (size_t i = 0; i < fsm->n; ++i)
        free(fsm->t[i]);
    free(fsm->t);
}

INLINE bool
suffix_fsm_switch(suffix_fsm_t *fsm, char c)
{
    fsm->q = fsm->t[fsm->q][c];
    return fsm->q == (fsm->n - 1);
}

SEARCH_FUNC(fsm_search)
{
    search_prepare_s_ss_len();

    suffix_fsm_t fsm;
    suffix_fsm_init(&fsm, ss, sslen);

    for (ssize_t i = 0; i < slen; ++i)
        if (suffix_fsm_switch(&fsm, s[i]))
            return i - sslen + 1;

    suffix_fsm_drop(&fsm);
    return -1;
}

static ssize_t *
kmp_compute_prefix(const char *p, ssize_t pn)
{
    assert(p && (pn > -1));

    ssize_t *f = malloc(pn * sizeof(ssize_t));
    f[0] = -1;

    ssize_t k = -1;
    for (ssize_t i = 1; i < pn; ++i)
    {
        while (k > -1 && (p[k + 1] != p[i]))
            k = f[k];
        if (p[k + 1] == p[i])
            ++k;
        f[i] = k;
    }

    return f;
}

SEARCH_FUNC(kmp_search)
{
    search_prepare_s_ss_len();
    ssize_t *kpf = kmp_compute_prefix(ss, sslen);

    ssize_t q = -1;
    for (ssize_t i = 0; i < slen; ++i)
    {
        while ((q > -1) && (ss[q + 1] != s[i]))
            q = kpf[q];
        if (ss[q + 1] == s[i])
            ++q;
        if ((q + 1) == sslen)
            return i - sslen + 1;
    }

    free(kpf);
    return -1;
}

static void
bm_full_match(ssize_t *sarr, ssize_t *barr, const char *p, ssize_t pn)
{
    assert(p && (pn > -1));

    ssize_t i = pn, j = pn + 1;
    barr[i] = j;

    while (i > 0)
    {
        while ((j <= pn) && (p[i - 1] != p[j - 1]))
        {
            if (!sarr[j])
                sarr[j] = j - i;
            j = barr[j];
        }

        --i; --j;
        barr[i] = j;
    }
}

static void
bm_partial_match(ssize_t *sarr, const ssize_t *barr, ssize_t pn)
{
    assert(pn > -1);

    ssize_t j = barr[0];
    for (ssize_t i = 0; i < pn; ++i)
    {
        if (!sarr[i])
            sarr[i] = j;
        if (i == j)
            j = barr[j];
    }
}

SEARCH_FUNC(boyer_moore_search) {
    search_prepare_s_ss_len();

    ssize_t *barr = malloc((sslen + 1) * sizeof(ssize_t));
    ssize_t *sarr = calloc(sslen + 1, sizeof(ssize_t));

    bm_full_match(sarr, barr, ss, sslen);
    bm_partial_match(sarr, barr, sslen);

    ssize_t shift = 0;
    while (shift <= (slen - sslen))
    {
        ssize_t j = sslen - 1;
        while ((j >= 0) && (ss[j] == s[shift + j]))
            --j;
        if (j < 0)
            break;

        shift += sarr[j + 1];
    }

    free(barr);
    free(sarr);

    return shift;
}
