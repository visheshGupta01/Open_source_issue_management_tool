#include "issueforge/issue_workflow.h"

#include <stdint.h>
#include <stdlib.h>

static int priority_rank(const char *priority) {
    if (priority[0] == 'c' || priority[0] == 'C') return 0;
    if (priority[0] == 'h' || priority[0] == 'H') return 1;
    if (priority[0] == 'm' || priority[0] == 'M') return 2;
    if (priority[0] == 'l' || priority[0] == 'L') return 3;
    return 4;
}

static int compare_issue(const Issue *a, const Issue *b, IFIssueSortKey key) {
    switch (key) {
        case IF_SORT_BY_ID:
            return (a->id > b->id) - (a->id < b->id);
        case IF_SORT_BY_PRIORITY: {
            int x = priority_rank(a->priority), y = priority_rank(b->priority);
            return (x > y) - (x < y);
        }
        case IF_SORT_BY_SEVERITY:
            return (b->severity > a->severity) - (b->severity < a->severity);
        case IF_SORT_BY_DEADLINE:
            return (a->deadline_days > b->deadline_days) -
                   (a->deadline_days < b->deadline_days);
        case IF_SORT_BY_ESTIMATED_HOURS:
            return (a->estimated_hours > b->estimated_hours) -
                   (a->estimated_hours < b->estimated_hours);
        default:
            return 0;
    }
}

static void merge(Issue items[], Issue temp[], size_t left,
                  size_t middle, size_t right, IFIssueSortKey key) {
    size_t i = left, j = middle, out = left;
    while (i < middle && j < right) {
        if (compare_issue(&items[i], &items[j], key) <= 0)
            temp[out++] = items[i++];
        else
            temp[out++] = items[j++];
    }
    while (i < middle) temp[out++] = items[i++];
    while (j < right) temp[out++] = items[j++];
    for (i = left; i < right; ++i) items[i] = temp[i];
}

static void merge_sort_range(Issue items[], Issue temp[], size_t left,
                             size_t right, IFIssueSortKey key) {
    if (right - left < 2) return;
    size_t middle = left + (right - left) / 2;
    merge_sort_range(items, temp, left, middle, key);
    merge_sort_range(items, temp, middle, right, key);
    merge(items, temp, left, middle, right, key);
}

int if_sort_issues(Issue issues[], size_t count, IFIssueSortKey key) {
    if ((!issues && count != 0) || key < IF_SORT_BY_ID ||
        key > IF_SORT_BY_ESTIMATED_HOURS ||
        count > SIZE_MAX / sizeof(Issue)) return -1;
    if (count < 2) return 0;
    Issue *temp = malloc(count * sizeof(*temp));
    if (!temp) return -1;
    merge_sort_range(issues, temp, 0, count, key);
    free(temp);
    return 0;
}

ptrdiff_t if_find_issue_by_id(const Issue issues[], size_t count, int issue_id) {
    if (!issues || count == 0 || count > (size_t)PTRDIFF_MAX) return -1;
    size_t low = 0, high = count;
    while (low < high) {
        size_t middle = low + (high - low) / 2;
        if (issues[middle].id == issue_id) return (ptrdiff_t)middle;
        if (issues[middle].id < issue_id) low = middle + 1;
        else high = middle;
    }
    return -1;
}
