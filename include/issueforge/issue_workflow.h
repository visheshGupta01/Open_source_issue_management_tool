#ifndef ISSUEFORGE_ISSUE_WORKFLOW_H
#define ISSUEFORGE_ISSUE_WORKFLOW_H

#include "issueforge/issueforge.h"
#include <stddef.h>

typedef enum {
    IF_SORT_BY_ID = 1,
    IF_SORT_BY_PRIORITY,
    IF_SORT_BY_SEVERITY,
    IF_SORT_BY_DEADLINE,
    IF_SORT_BY_ESTIMATED_HOURS
} IFIssueSortKey;

/* Stable merge sort. Equal-key issues retain their relative input order. */
int if_sort_issues(Issue issues[], size_t count, IFIssueSortKey key);

/* Binary search on an array already sorted by ascending issue ID.
   Returns the zero-based index, or -1 when the ID is not present. */
ptrdiff_t if_find_issue_by_id(const Issue issues[], size_t count, int issue_id);

#endif
