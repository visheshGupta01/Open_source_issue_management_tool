#include "issueforge/issue_workflow.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void sorting_and_searching(void) {
    RepositoryData data = {0};
    char error[IF_TEXT];
    assert(if_load_repository("data/sample/issues.csv",
                              "data/sample/developers.csv",
                              &data, error, sizeof(error)) == 0);
    assert(data.issue_count == 8);

    assert(if_sort_issues(data.issues, data.issue_count,
                          IF_SORT_BY_PRIORITY) == 0);
    assert(strcmp(data.issues[0].priority, "critical") == 0);
    assert(strcmp(data.issues[1].priority, "high") == 0);
    assert(strcmp(data.issues[2].priority, "high") == 0);
    assert(strcmp(data.issues[4].priority, "medium") == 0);

    assert(if_sort_issues(data.issues, data.issue_count,
                          IF_SORT_BY_DEADLINE) == 0);
    for (size_t i = 1; i < data.issue_count; ++i)
        assert(data.issues[i - 1].deadline_days <= data.issues[i].deadline_days);
    assert(data.issues[0].id == 103);
    assert(data.issues[1].id == 101);

    assert(if_sort_issues(data.issues, data.issue_count, IF_SORT_BY_ID) == 0);
    for (size_t i = 1; i < data.issue_count; ++i)
        assert(data.issues[i - 1].id < data.issues[i].id);
    ptrdiff_t found = if_find_issue_by_id(data.issues, data.issue_count, 106);
    assert(found >= 0 && data.issues[found].id == 106);
    assert(if_find_issue_by_id(data.issues, data.issue_count, 999) == -1);
    assert(if_find_issue_by_id(NULL, 0, 1) == -1);
    assert(if_sort_issues(NULL, 0, IF_SORT_BY_ID) == 0);
    assert(if_sort_issues(data.issues, data.issue_count,
                          (IFIssueSortKey)99) == -1);
}

int main(void) {
    sorting_and_searching();
    puts("Issue workflow integration tests passed.");
    return 0;
}
