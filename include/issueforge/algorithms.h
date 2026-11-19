#ifndef ISSUEFORGE_ALGORITHMS_H
#define ISSUEFORGE_ALGORITHMS_H
#include <stddef.h>
#include <stdint.h>
typedef struct { int64_t sum; size_t start; size_t end; } IFSubarrayResult;
void if_merge_sort(int values[], size_t count);
void if_quick_sort(int values[], size_t count);
ptrdiff_t if_binary_search_iterative(const int values[], size_t count, int target);
ptrdiff_t if_binary_search_recursive(const int values[], size_t count, int target);
int if_maximum_subarray(const int values[], size_t count, IFSubarrayResult *result);
int if_fast_power(double base, int exponent, double *result);
ptrdiff_t if_find_celebrity(const unsigned char *knows, size_t count);
int if_matrix_multiply_classical(const int64_t *a, const int64_t *b, int64_t *out, size_t count);
int if_matrix_multiply_strassen(const int64_t *a, const int64_t *b, int64_t *out, size_t count);
#endif
