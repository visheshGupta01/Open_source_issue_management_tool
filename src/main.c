#include "issueforge/issueforge.h"
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
    puts("0. Exit");
    printf("Choose an option: ");
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
                    printf("Initial validation: %zu issue(s) found.\n", errors.count);
            }
        } else if (choice >= 2 && choice <= 4) {
            if (!loaded) {
                puts("Load the sample data first using option 1.");
                continue;
            }

            if (choice == 2) {
                puts("\nISSUES");
                for (size_t i = 0; i < data.issue_count; ++i) {
                    const Issue *issue = &data.issues[i];
                    printf("#%d | %-30s | %-8s | %-10s | %dh\n",
                           issue->id, issue->title, issue->priority,
                           issue->status, issue->estimated_hours);
                }
            } else if (choice == 3) {
                puts("\nDEVELOPERS");
                for (size_t i = 0; i < data.developer_count; ++i) {
                    const Developer *developer = &data.developers[i];
                    printf("%d | %-20s | %dh available | %d years experience\n",
                           developer->id, developer->name,
                           developer->available_hours, developer->experience_years);
                }
            } else {
                if (if_validate_repository(&data, &errors) == 0) {
                    puts("Repository validation passed. No problems found.");
                } else {
                    printf("Validation found %zu problem(s):\n", errors.count);
                    for (size_t i = 0; i < errors.count; ++i)
                        printf(" - %s\n", errors.messages[i]);
                }
            }
        } else {
            puts("Unknown option. Try again.");
        }
    }

    return 0;
}
