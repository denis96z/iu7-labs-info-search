#include <stdio.h>
#include <string.h>

#include "task.h"

void
print_usage(FILE *stream, const char *cmd,
            const char *panic_file, int panic_line)
{
    if (panic_file && panic_line)
        fprintf(stream, "panic: %s:%d\n", panic_file, panic_line);

    fprintf(stream, "%s "
            "(--bubble|--insertion|--tree|--quick) "
            "(--sorted|--rsorted|--random) "
            "(--n=) (--min=) (--max=)\n", cmd);
}

#define panic() \
    ({                                                        \
        print_usage(stderr, argv[0], __FILE_REL__, __LINE__); \
        return 1;                                             \
    })

#define panic_msg(msg) \
    ({                                                              \
        fprintf(stderr, "%s (%s:%d)", msg, __FILE_REL__, __LINE__); \
        return 2;                                                   \
    })

#define parse_prefixed_arg(arg, prefix) \
    ({                                             \
        if (strlen(arg) < sizeof(prefix)) panic(); \
        (arg) + sizeof(prefix) - 1;                \
    })

int
main(int argc, const char **argv)
{
    if (argc != 6)
        panic();

    const char *sort_method   = argv[1];
    const char *initial_order = argv[2];

    const char *num_items = parse_prefixed_arg(argv[3], "--n=");
    const char *min_value = parse_prefixed_arg(argv[4], "--min=");
    const char *max_value = parse_prefixed_arg(argv[5], "--max=");

    sort_func_t sort_func = NULL;
    if (!strcmp(sort_method, "--bubble"))
        sort_func = bubble_sort;
    else if (!strcmp(sort_method, "--quick"))
        sort_func = quick_sort;
    else if (!strcmp(sort_method, "--tree"))
        sort_func = tree_sort;
    else if (!strcmp(sort_method, "--insertion"))
        sort_func = insertion_sort;
    else
        panic();

    order_func_t order_func = NULL;
    if (!strcmp(initial_order, "--sorted"))
        order_func = make_sorted;
    else if (!strcmp(initial_order, "--rsorted"))
        order_func = make_rsorted;
    else if (!strcmp(initial_order, "--random"))
        order_func = shuffle;
    else
        panic();

    size_t n = (size_t)strtoul(num_items, NULL, 10);
    if (!n)
        panic();

    int_t vmin = strtol(min_value, NULL, 10);
    int_t vmax = strtol(max_value, NULL, 10);
    if (vmin > vmax)
        panic();

    int_arr_t arr = { 0 };
    if (!init_int_arr_range(&arr, n, vmin, vmax))
        panic_msg("array initialization fail!");

    order_int_arr(&arr, order_func);
    int_arr_sort_stat_t stat = sort_int_arr(&arr, sort_func);

    drop_int_arr(&arr);

    printf("STAT\ncmp: %lu\nswap: %lu\n", stat.ncmp, stat.nswap);
    return 0;
}
