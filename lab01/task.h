#ifndef LAB01_TASK_H
#define LAB01_TASK_H

#include "sort.h"

typedef int int_t;

typedef struct
{
    size_t n;
    int_t *items;
}
int_arr_t;

int_arr_t *
init_int_arr(int_arr_t *arr, size_t n);

int_arr_t *
init_int_arr_range(int_arr_t *arr, size_t n,
                   int_t min_value, int_t max_value);

void
drop_int_arr(int_arr_t *arr);

void
order_int_arr(int_arr_t *arr, order_func_t order_func);

typedef struct
{
    size_t ncmp;
    size_t nswap;
}
int_arr_sort_stat_t;

int_arr_sort_stat_t
sort_int_arr(int_arr_t *arr, sort_func_t sort_func);

#endif
