#include "issueforge/assignment.h"
#include "issueforge/issue_workflow.h"

#include <ctype.h>
#include <string.h>

static int same_text(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        ++a;
        ++b;
    }
    return *a == '\0' && *b == '\0';
}

static int skill_matches_issue(const char *skill, const Issue *issue) {
    if (same_text(skill, issue->module)) return 1;

    for (size_t i = 0; i < issue->label_count; ++i) {
        if (same_text(skill, issue->labels[i])) return 1;
    }
    return 0;
}

static int count_skill_matches(const Developer *developer,
                               const Issue *issue) {
    int matches = 0;

    for (size_t i = 0; i < developer->skill_count; ++i) {
        if (skill_matches_issue(developer->skills[i], issue))
            ++matches;
    }
    return matches;
}

static int is_assignable_status(const char *status) {
    return same_text(status, "open") || same_text(status, "in_progress");
}

static int is_skipped_status(const char *status) {
    return same_text(status, "blocked") || same_text(status, "closed");
}

static ptrdiff_t choose_developer(const RepositoryData *data,
                                  const Issue *issue) {
    ptrdiff_t best = -1;
    int best_matches = 0;

    for (size_t i = 0; i < data->developer_count; ++i) {
        const Developer *candidate = &data->developers[i];
        int matches;

        if (candidate->available_hours < issue->estimated_hours)
            continue;

        matches = count_skill_matches(candidate, issue);
        if (matches == 0) continue;

        if (best < 0 ||
            matches > best_matches ||
            (matches == best_matches &&
             candidate->available_hours >
                 data->developers[best].available_hours) ||
            (matches == best_matches &&
             candidate->available_hours ==
                 data->developers[best].available_hours &&
             candidate->experience_years >
                 data->developers[best].experience_years)) {
            best = (ptrdiff_t)i;
            best_matches = matches;
        }
    }

    return best;
}

int if_assign_issues_greedy(RepositoryData *data,
                            IFAssignmentSummary *summary) {
    IFAssignmentSummary result = {0};

    if (!data || !summary ||
        data->issue_count > IF_MAX_ISSUES ||
        data->developer_count > IF_MAX_DEVELOPERS)
        return -1;

    for (size_t i = 0; i < data->issue_count; ++i) {
        if (data->issues[i].label_count > IF_LIST ||
            data->issues[i].estimated_hours <= 0)
            return -1;
    }
    for (size_t i = 0; i < data->developer_count; ++i) {
        if (data->developers[i].skill_count > IF_LIST ||
            data->developers[i].available_hours < 0)
            return -1;
    }

    if (if_sort_issues(data->issues, data->issue_count,
                       IF_SORT_BY_PRIORITY) != 0)
        return -1;

    for (size_t i = 0; i < data->issue_count; ++i) {
        Issue *issue = &data->issues[i];
        ptrdiff_t selected;

        if (issue->assigned_developer_id != 0) {
            ++result.assigned_count;
            continue;
        }

        if (is_skipped_status(issue->status)) {
            ++result.skipped_count;
            continue;
        }

        if (!is_assignable_status(issue->status)) {
            ++result.skipped_count;
            continue;
        }

        selected = choose_developer(data, issue);
        if (selected < 0) {
            ++result.unassigned_count;
            continue;
        }

        Developer *developer = &data->developers[selected];
        issue->assigned_developer_id = developer->id;
        developer->available_hours -= issue->estimated_hours;
        ++result.assigned_count;
    }

    *summary = result;
    return 0;
}
