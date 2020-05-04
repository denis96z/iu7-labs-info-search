#include <stdio.h>

#include "search.h"

void
print_usage(FILE *stream, const char *cmd,
            const char *panic_file, int panic_line)
{
    if (panic_file && panic_line)
        fprintf(stream, "panic: %s:%d\n", panic_file, panic_line);
    fprintf(stream, "%s (string) (substring) "
                    "(--naive|--rk|--fsm|--kmp|--bm)\n", cmd);
}

#define panic() \
    ({                                                        \
        print_usage(stderr, argv[0], __FILE_REL__, __LINE__); \
        return 1;                                             \
    })

int
main(int argc, const char **argv)
{
    if (argc != 4)
        panic();

    const char *str       = argv[1];
    const char *substr    = argv[2];
    const char *algorithm = argv[3];

    search_func_t search_func = NULL;
    if (!strcmp(algorithm, "--naive"))
        search_func = naive_search;
    else if (!strcmp(algorithm, "--rk"))
        search_func = rabin_karp_search;
    else if (!strcmp(algorithm, "--fsm"))
        search_func = fsm_search;
    else if (!strcmp(algorithm, "--kmp"))
        search_func = kmp_search;
    else if (!strcmp(algorithm, "--bm"))
        search_func = boyer_moore_search;
    else
        panic();

    ssize_t idx = search_func(str, -1, substr, -1);
    printf("%zd\n", idx);

    return 0;
}
