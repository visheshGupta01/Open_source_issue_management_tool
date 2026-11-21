#ifndef ISSUEFORGE_ASSIGNMENT_H
#define ISSUEFORGE_ASSIGNMENT_H

#include "issueforge/issueforge.h"

typedef struct {
    size_t assigned_count;   /* Total issues assigned after this run */
    size_t unassigned_count; /* Eligible issues with no suitable developer */
    size_t skipped_count;    /* Blocked or closed issues */
} IFAssignmentSummary;

/*
 * Greedy assignment:
 * - processes issues from highest to lowest priority using Merge Sort;
 * - matches developer skills against an issue's module and labels;
 * - requires enough remaining developer hours;
 * - chooses the strongest skill match, then the most available hours,
 *   then the most experienced developer.
 *
 * Existing assignments are preserved, so calling this function again
 * does not deduct the same issue hours twice.
 */
int if_assign_issues_greedy(RepositoryData *data,
                            IFAssignmentSummary *summary);

#endif
