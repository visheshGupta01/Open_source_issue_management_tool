#include "issueforge/assignment.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static Issue *find_issue(RepositoryData *data, int id) {
    for (size_t i = 0; i < data->issue_count; ++i)
        if (data->issues[i].id == id) return &data->issues[i];
    return NULL;
}

static Developer *find_developer(RepositoryData *data, int id) {
    for (size_t i = 0; i < data->developer_count; ++i)
        if (data->developers[i].id == id) return &data->developers[i];
    return NULL;
}

int main(void) {
    RepositoryData data = {0};
    IFAssignmentSummary summary = {0};
    char error[IF_TEXT];

    assert(if_load_repository("data/sample/issues.csv",
                              "data/sample/developers.csv",
                              &data, error, sizeof(error)) == 0);

    assert(if_assign_issues_greedy(&data, &summary) == 0);
    assert(summary.assigned_count == 5);
    assert(summary.unassigned_count == 2);
    assert(summary.skipped_count == 1);

    /* Critical issue 103 is assigned first; available hours break skill ties. */
    assert(find_issue(&data, 103)->assigned_developer_id == 1);
    assert(find_issue(&data, 101)->assigned_developer_id == 3);
    assert(find_issue(&data, 104)->assigned_developer_id == 1);
    assert(find_issue(&data, 102)->assigned_developer_id == 2);
    assert(find_issue(&data, 105)->assigned_developer_id == 4);

    /* No sample developer has the required skills for these issues. */
    assert(find_issue(&data, 107)->assigned_developer_id == 0);
    assert(find_issue(&data, 108)->assigned_developer_id == 0);

    /* Blocked issues are not assigned. */
    assert(find_issue(&data, 106)->assigned_developer_id == 0);

    assert(find_developer(&data, 1)->available_hours == 2);
    assert(find_developer(&data, 2)->available_hours == 8);
    assert(find_developer(&data, 3)->available_hours == 5);
    assert(find_developer(&data, 4)->available_hours == 5);

    /* A second run must not assign again or deduct hours twice. */
    assert(if_assign_issues_greedy(&data, &summary) == 0);
    assert(summary.assigned_count == 5);
    assert(summary.unassigned_count == 2);
    assert(summary.skipped_count == 1);
    assert(find_developer(&data, 1)->available_hours == 2);

    puts("Greedy assignment tests passed.");
    return 0;
}
