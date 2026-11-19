#include "issueforge/issueforge.h"
#include "issueforge/algorithms.h"
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
    puts("0. Exit");
    printf("Choose an option: ");
}

static void run_algorithm_lab(void) {
    int values[] = {9, -2, 7, 7, 0, 4, 1};
    int sorted[7];
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
    if (if_maximum_subarray(workload, 9, &best) == 0)
        printf("Maximum subarray: sum=%lld, indices=%zu..%zu\n",
               (long long)best.sum, best.start, best.end);

    double power;
    if (if_fast_power(2.0, 10, &power) == 0)
        printf("Fast exponentiation: 2^10 = %.0f\n", power);

    const unsigned char knows[] = {0,1,1, 0,0,1, 0,0,0};
    printf("Celebrity in sample group: person %td\n", if_find_celebrity(knows, 3));

    const int64_t a[] = {1,2,3,4,5,6,7,8,9};
    const int64_t b[] = {9,8,7,6,5,4,3,2,1};
    int64_t classical[9], strassen[9];
    if (if_matrix_multiply_classical(a,b,classical,3) == 0 &&
        if_matrix_multiply_strassen(a,b,strassen,3) == 0) {
        printf("Matrix multiplication (classical vs Strassen): %s\n",
               classical[0] == strassen[0] && classical[8] == strassen[8]
                   ? "results match" : "results differ");
    }
    puts("See docs/phase2-algorithms.md for algorithm analysis and complexity.");
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
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            puts("Please enter a valid number.");
            continue;
        }
        if (choice == 0) { puts("Goodbye."); break; }
        if (choice == 1) {
            if (if_load_repository("data/sample/issues.csv", "data/sample/developers.csv",
                                   &data, error, sizeof(error)) != 0) {
                printf("Could not load data: %s\n", error); loaded = 0;
            } else {
                loaded = 1;
                printf("Loaded %zu issues and %zu developers.\n", data.issue_count, data.developer_count);
                if (if_validate_repository(&data, &errors) == 0) puts("Initial validation: passed.");
                else printf("Initial validation: %zu issue(s) found.\n", errors.count);
            }
        } else if (choice >= 2 && choice <= 4) {
            if (!loaded) { puts("Load the sample data first using option 1."); continue; }
            if (choice == 2) {
                puts("\nISSUES");
                for (size_t i=0;i<data.issue_count;++i) {
                    const Issue *x=&data.issues[i];
                    printf("#%d | %-30s | %-8s | %-10s | %dh\n",x->id,x->title,x->priority,x->status,x->estimated_hours);
                }
            } else if (choice == 3) {
                puts("\nDEVELOPERS");
                for (size_t i=0;i<data.developer_count;++i) {
                    const Developer *x=&data.developers[i];
                    printf("%d | %-20s | %dh available | %d years experience\n",x->id,x->name,x->available_hours,x->experience_years);
                }
            } else if (if_validate_repository(&data,&errors)==0) puts("Repository validation passed. No problems found.");
            else {
                printf("Validation found %zu problem(s):\n",errors.count);
                for(size_t i=0;i<errors.count;++i)printf(" - %s\n",errors.messages[i]);
            }
        } else if (choice == 5) run_algorithm_lab();
        else puts("Unknown option. Try again.");
    }
    return 0;
}
