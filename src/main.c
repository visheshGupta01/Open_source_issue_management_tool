#include "issueforge/issueforge.h"
#include "issueforge/algorithms.h"
#include "issueforge/issue_workflow.h"

#include <stdio.h>

static void display_menu(void) {
    puts("\n========================================");
    puts("                 ISSUEFORGE");
    puts("       Open Source Issue Triage Tool");
    puts("========================================");
    puts("1. Load sample repository data");
    puts("2. View issues");
    puts("3. View developers");
    puts("4. Validate repository");
    puts("5. Run Phase 2 algorithm laboratory");
    puts("6. Sort real issues (Merge Sort)");
    puts("7. Search issue by ID (Binary Search)");
    puts("0. Exit");
    printf("Choose an option: ");
}

static void print_issues(const RepositoryData *data) {
    puts("\nID   PRIORITY  SEVERITY  DEADLINE  HOURS  STATUS       TITLE");
    puts("--------------------------------------------------------------------------");
    for (size_t i = 0; i < data->issue_count; ++i) {
        const Issue *x = &data->issues[i];
        printf("%-4d %-9s %-9d %-9d %-6d %-12s %s\n",
               x->id, x->priority, x->severity, x->deadline_days,
               x->estimated_hours, x->status, x->title);
    }
}

static void run_algorithm_lab(void) {
    int values[] = {9, -2, 7, 7, 0, 4, 1}, sorted[7];
    for (size_t i = 0; i < 7; ++i) sorted[i] = values[i];
    if_merge_sort(sorted, 7);
    puts("\nPHASE 2 — ALGORITHM LAB");
    printf("Merge sort: ");
    for (size_t i = 0; i < 7; ++i) printf("%d ", sorted[i]);
    printf("\nQuick sort: ");
    for (size_t i = 0; i < 7; ++i) sorted[i] = values[i];
    if_quick_sort(sorted, 7);
    for (size_t i = 0; i < 7; ++i) printf("%d ", sorted[i]);
    printf("\nBinary search for 4: index %td (iterative), %td (recursive)\n",
           if_binary_search_iterative(sorted, 7, 4),
           if_binary_search_recursive(sorted, 7, 4));
    int workload[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    IFSubarrayResult best;
    if (!if_maximum_subarray(workload, 9, &best))
        printf("Maximum subarray: sum=%lld, indices=%zu..%zu\n",
               (long long)best.sum, best.start, best.end);
    double power;
    if (!if_fast_power(2.0, 10, &power))
        printf("Fast exponentiation: 2^10 = %.0f\n", power);
    const unsigned char knows[] = {0,1,1,0,0,1,0,0,0};
    printf("Celebrity in sample group: person %td\n",
           if_find_celebrity(knows, 3));
    const int64_t a[] = {1,2,3,4,5,6,7,8,9};
    const int64_t b[] = {9,8,7,6,5,4,3,2,1};
    int64_t c[9], s[9];
    if (!if_matrix_multiply_classical(a,b,c,3) &&
        !if_matrix_multiply_strassen(a,b,s,3))
        printf("Matrix multiplication (classical vs Strassen): %s\n",
               c[0] == s[0] && c[8] == s[8] ?
               "results match" : "results differ");
    puts("See docs/phase2-algorithms.md for complexity analysis.");
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
    if (choice < 1 || choice > 5) {
        puts("Invalid sort option.");
        return;
    }
    if (if_sort_issues(data->issues, data->issue_count,
                       (IFIssueSortKey)choice) != 0) {
        puts("Could not sort issues (invalid input or insufficient memory).");
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
    /* Binary search requires ascending IDs, so prepare the real issue array. */
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
}

int main(void) {
    RepositoryData data = {0};
    ValidationErrors errors = {0};
    char error[IF_TEXT];
    int loaded = 0, choice;

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
                if (!if_validate_repository(&data, &errors))
                    puts("Initial validation: passed.");
                else
                    printf("Initial validation: %zu issue(s) found.\n",
                           errors.count);
            }
        } else if (choice == 5) {
            run_algorithm_lab();
        } else if (choice >= 2 && choice <= 7) {
            if (!loaded) {
                puts("Load the sample data first using option 1.");
                continue;
            }
            if (choice == 2) {
                puts("\nISSUES");
                print_issues(&data);
            } else if (choice == 3) {
                puts("\nDEVELOPERS");
                for (size_t i = 0; i < data.developer_count; ++i) {
                    const Developer *x = &data.developers[i];
                    printf("%d | %-20s | %dh available | %d years experience\n",
                           x->id, x->name, x->available_hours,
                           x->experience_years);
                }
            } else if (choice == 4) {
                if (!if_validate_repository(&data, &errors))
                    puts("Repository validation passed. No problems found.");
                else {
                    printf("Validation found %zu problem(s):\n", errors.count);
                    for (size_t i = 0; i < errors.count; ++i)
                        printf(" - %s\n", errors.messages[i]);
                }
            } else if (choice == 6) {
                sort_real_issues(&data);
            } else if (choice == 7) {
                search_real_issue(&data);
            }
        } else {
            puts("Unknown option. Try again.");
        }
    }
    return 0;
}
