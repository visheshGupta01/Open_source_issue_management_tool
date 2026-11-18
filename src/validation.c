#include "issueforge/issueforge.h"
#include <stdio.h>
#include <string.h>

static void add_error(ValidationErrors *errors, const char *message) {
    if (errors->count < IF_MAX_ERRORS)
        snprintf(errors->messages[errors->count++], IF_TEXT, "%s", message);
}

static int find_issue_index(const RepositoryData *data, int id) {
    for (size_t i = 0; i < data->issue_count; ++i)
        if (data->issues[i].id == id) return (int)i;
    return -1;
}

static void visit_dependency_graph(const RepositoryData *data, size_t index,
                                   unsigned char state[], ValidationErrors *errors) {
    state[index] = 1;

    const Issue *issue = &data->issues[index];
    for (size_t j = 0; j < issue->dependency_count; ++j) {
        int next = find_issue_index(data, issue->dependencies[j]);
        if (next < 0) continue;

        if (state[next] == 1) {
            char message[IF_TEXT];
            snprintf(message, sizeof(message),
                     "Dependency cycle detected at issue %d", data->issues[next].id);
            add_error(errors, message);
        } else if (state[next] == 0) {
            visit_dependency_graph(data, (size_t)next, state, errors);
        }
    }

    state[index] = 2;
}

int if_validate_repository(const RepositoryData *data, ValidationErrors *errors) {
    unsigned char state[IF_MAX_ISSUES] = {0};

    if (!data || !errors || data->issue_count > IF_MAX_ISSUES ||
        data->developer_count > IF_MAX_DEVELOPERS) return -1;

    errors->count = 0;

    for (size_t i = 0; i < data->issue_count; ++i) {
        const Issue *issue = &data->issues[i];
        char message[IF_TEXT];

        if (issue->id <= 0) {
            snprintf(message, sizeof(message), "Issue ID must be positive: %d", issue->id);
            add_error(errors, message);
        }
        for (size_t j = 0; j < i; ++j) {
            if (data->issues[j].id == issue->id) {
                snprintf(message, sizeof(message), "Duplicate issue ID: %d", issue->id);
                add_error(errors, message);
                break;
            }
        }
        if (!issue->title[0]) {
            snprintf(message, sizeof(message), "Issue %d has an empty title", issue->id);
            add_error(errors, message);
        }
        if (issue->estimated_hours <= 0) {
            snprintf(message, sizeof(message), "Issue %d must require positive hours", issue->id);
            add_error(errors, message);
        }
        if (issue->severity < 1 || issue->severity > 10) {
            snprintf(message, sizeof(message), "Issue %d severity must be 1..10", issue->id);
            add_error(errors, message);
        }
        if (issue->deadline_days < 0) {
            snprintf(message, sizeof(message), "Issue %d has a negative deadline", issue->id);
            add_error(errors, message);
        }
        if (strcmp(issue->status, "open") && strcmp(issue->status, "in_progress") &&
            strcmp(issue->status, "blocked") && strcmp(issue->status, "closed")) {
            snprintf(message, sizeof(message), "Issue %d has invalid status", issue->id);
            add_error(errors, message);
        }
        if (strcmp(issue->priority, "low") && strcmp(issue->priority, "medium") &&
            strcmp(issue->priority, "high") && strcmp(issue->priority, "critical")) {
            snprintf(message, sizeof(message), "Issue %d has invalid priority", issue->id);
            add_error(errors, message);
        }

        for (size_t j = 0; j < issue->dependency_count; ++j) {
            if (issue->dependencies[j] == issue->id) {
                snprintf(message, sizeof(message), "Issue %d depends on itself", issue->id);
                add_error(errors, message);
            }
            if (find_issue_index(data, issue->dependencies[j]) < 0) {
                snprintf(message, sizeof(message),
                         "Issue %d references missing dependency %d",
                         issue->id, issue->dependencies[j]);
                add_error(errors, message);
            }
            for (size_t k = 0; k < j; ++k) {
                if (issue->dependencies[k] == issue->dependencies[j]) {
                    snprintf(message, sizeof(message),
                             "Issue %d repeats dependency %d",
                             issue->id, issue->dependencies[j]);
                    add_error(errors, message);
                    break;
                }
            }
        }
    }

    for (size_t i = 0; i < data->developer_count; ++i) {
        const Developer *developer = &data->developers[i];
        char message[IF_TEXT];

        if (developer->id <= 0) {
            snprintf(message, sizeof(message), "Developer ID must be positive: %d", developer->id);
            add_error(errors, message);
        }
        for (size_t j = 0; j < i; ++j) {
            if (data->developers[j].id == developer->id) {
                snprintf(message, sizeof(message), "Duplicate developer ID: %d", developer->id);
                add_error(errors, message);
                break;
            }
        }
        if (!developer->name[0]) {
            snprintf(message, sizeof(message), "Developer %d has an empty name", developer->id);
            add_error(errors, message);
        }
        if (developer->available_hours < 0) {
            snprintf(message, sizeof(message), "Developer %d has negative availability", developer->id);
            add_error(errors, message);
        }
        if (developer->experience_years < 0) {
            snprintf(message, sizeof(message), "Developer %d has negative experience", developer->id);
            add_error(errors, message);
        }
    }

    for (size_t i = 0; i < data->issue_count; ++i)
        if (state[i] == 0) visit_dependency_graph(data, i, state, errors);

    return errors->count == 0 ? 0 : 1;
}
