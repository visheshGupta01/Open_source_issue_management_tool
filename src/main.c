#include "issueforge/issueforge.h"
#include "issueforge/issue_workflow.h"
#include "issueforge/assignment.h"

#include <stdio.h>

static void display_menu(void) {
    puts("\n========================================");
    puts("                 ISSUEFORGE");
    puts("       Open Source Issue Triage Tool");
    puts("========================================");
    puts("1. Load sample repository data");
    puts("2. View issues");
    puts("3. View developers");
    puts("4. Validate repository (including dependency cycles)");
    puts("5. Sort real issues (Merge Sort)");
    puts("6. Search issue by ID (Binary Search)");
    puts("7. Assign issues to developers (Greedy Algorithm)");
    puts("8. View assignment results");
    puts("0. Exit");
    printf("Choose an option: ");
}

static const char *developer_name(const RepositoryData *data, int id) {
    for (size_t i = 0; i < data->developer_count; ++i) {
        if (data->developers[i].id == id)
            return data->developers[i].name;
    }
    return "Unassigned";
}

static void print_issues(const RepositoryData *data) {
    puts("\nID   PRIORITY  SEVERITY  DEADLINE  HOURS  ASSIGNED TO       STATUS       TITLE");
    puts("------------------------------------------------------------------------------------------");
    for (size_t i = 0; i < data->issue_count; ++i) {
        const Issue *x = &data->issues[i];
        printf("%-4d %-9s %-9d %-9d %-6d %-17s %-12s %s\n",
               x->id, x->priority, x->severity, x->deadline_days,
               x->estimated_hours,
               developer_name(data, x->assigned_developer_id),
               x->status, x->title);
    }
}

static void print_assignment_results(const RepositoryData *data) {
    puts("\nDEVELOPER ASSIGNMENT RESULTS");
    puts("Issue  Developer          Hours");
    puts("--------------------------------");
    for (size_t i = 0; i < data->issue_count; ++i) {
        const Issue *issue = &data->issues[i];
        if (issue->assigned_developer_id != 0) {
            printf("#%-5d %-18s %dh\n", issue->id,
                   developer_name(data, issue->assigned_developer_id),
                   issue->estimated_hours);
        } else {
            printf("#%-5d %-18s %dh (%s)\n", issue->id, "Unassigned",
                   issue->estimated_hours, issue->status);
        }
    }

    puts("\nUPDATED DEVELOPER CAPACITY");
    for (size_t i = 0; i < data->developer_count; ++i) {
        const Developer *developer = &data->developers[i];
        printf("%-18s %d hour(s) remaining\n",
               developer->name, developer->available_hours);
    }
}

static void sort_real_issues(RepositoryData *data) {
    int choice;
    puts("\nSort issues by:");
    puts("1. Issue ID (ascending)");
    puts("2. Priority (critical to low)");
    puts("3. Severity (highest first)");
    puts("4. Deadline (soonest first)");
    puts("5. Estimated effort (lowest first)");
    printf("Choose a sort key: ");

    if (scanf("%d", &choice) != 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        puts("Invalid input.");
        return;
    }

    if (choice < IF_SORT_BY_ID || choice > IF_SORT_BY_ESTIMATED_HOURS) {
        puts("Invalid sort option.");
        return;
    }

    if (if_sort_issues(data->issues, data->issue_count,
                       (IFIssueSortKey)choice) != 0) {
        puts("Could not sort issues.");
        return;
    }

    puts("\nIssues sorted successfully using Merge Sort.");
    print_issues(data);
}

static void search_real_issue(RepositoryData *data) {
    int id;
    printf("Enter issue ID to search: ");

    if (scanf("%d", &id) != 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        puts("Invalid issue ID.");
        return;
    }

    if (if_sort_issues(data->issues, data->issue_count, IF_SORT_BY_ID) != 0) {
        puts("Could not prepare issue list for searching.");
        return;
    }

    ptrdiff_t index = if_find_issue_by_id(data->issues, data->issue_count, id);
    if (index < 0) {
        printf("Issue #%d was not found.\n", id);
        return;
    }

    const Issue *x = &data->issues[index];
    puts("\nISSUE FOUND");
    printf("ID:              %d\n", x->id);
    printf("Title:           %s\n", x->title);
    printf("Status:          %s\n", x->status);
    printf("Priority:        %s\n", x->priority);
    printf("Module:          %s\n", x->module);
    printf("Severity:        %d/10\n", x->severity);
    printf("Estimated hours: %d\n", x->estimated_hours);
    printf("Deadline:        %d day(s)\n", x->deadline_days);
    printf("Assigned to:     %s\n",
           developer_name(data, x->assigned_developer_id));
}

static void assign_issues(RepositoryData *data) {
    IFAssignmentSummary summary;

    if (if_assign_issues_greedy(data, &summary) != 0) {
        puts("Assignment could not be completed. Check the repository data.");
        return;
    }

    puts("\nGreedy assignment completed.");
    printf("Assigned issues:   %zu\n", summary.assigned_count);
    printf("Unassigned issues: %zu (no matching skill or enough capacity)\n",
           summary.unassigned_count);
    printf("Skipped issues:    %zu (blocked or closed)\n",
           summary.skipped_count);
    print_assignment_results(data);
}

int main(void) {
    RepositoryData data = {0};
    ValidationErrors errors = {0};
    char error[IF_TEXT];
    int loaded = 0;
    int choice;

    for (;;) {
        display_menu();

        if (scanf("%d", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            puts("Please enter a valid number.");
            continue;
        }

        if (choice == 0) {
            puts("Goodbye.");
            break;
        }

        if (choice == 1) {
            if (if_load_repository("data/sample/issues.csv",
                                   "data/sample/developers.csv",
                                   &data, error, sizeof(error)) != 0) {
                printf("Could not load data: %s\n", error);
                loaded = 0;
            } else {
                loaded = 1;
                printf("Loaded %zu issues and %zu developers.\n",
                       data.issue_count, data.developer_count);

                if (if_validate_repository(&data, &errors) == 0)
                    puts("Initial validation: passed.");
                else
                    printf("Initial validation: %zu problem(s) found.\n",
                           errors.count);
            }
            continue;
        }

        if (choice < 2 || choice > 8) {
            puts("Unknown option. Try again.");
            continue;
        }

        if (!loaded) {
            puts("Load the sample data first using option 1.");
            continue;
        }

        switch (choice) {
            case 2:
                puts("\nISSUES");
                print_issues(&data);
                break;

            case 3:
                puts("\nDEVELOPERS");
                for (size_t i = 0; i < data.developer_count; ++i) {
                    const Developer *x = &data.developers[i];
                    printf("%d | %-20s | %dh available | %d years experience\n",
                           x->id, x->name, x->available_hours,
                           x->experience_years);
                }
                break;

            case 4:
                if (if_validate_repository(&data, &errors) == 0) {
                    puts("Repository validation passed. No problems found.");
                } else {
                    printf("Validation found %zu problem(s):\n", errors.count);
                    for (size_t i = 0; i < errors.count; ++i)
                        printf(" - %s\n", errors.messages[i]);
                }
                break;

            case 5:
                sort_real_issues(&data);
                break;

            case 6:
                search_real_issue(&data);
                break;

            case 7:
                assign_issues(&data);
                break;

            case 8:
                print_assignment_results(&data);
                break;
        }
    }

    return 0;
}
