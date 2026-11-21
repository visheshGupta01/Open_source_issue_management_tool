#ifndef ISSUEFORGE_H
#define ISSUEFORGE_H

#include <stddef.h>

#define IF_TEXT 256
#define IF_LIST 16
#define IF_MAX_ISSUES 512
#define IF_MAX_DEVELOPERS 128
#define IF_MAX_ERRORS 1024

typedef struct {
    int id;
    char title[IF_TEXT];
    char status[32];
    char priority[32];
    char module[64];
    int estimated_hours;
    int severity;
    int deadline_days;
    char labels[IF_LIST][64];
    size_t label_count;
    int dependencies[IF_LIST];
    size_t dependency_count;
    int assigned_developer_id; /* 0 means not assigned */
} Issue;

typedef struct {
    int id;
    char name[IF_TEXT];
    char skills[IF_LIST][64];
    size_t skill_count;
    int available_hours;
    int experience_years;
} Developer;

typedef struct {
    Issue issues[IF_MAX_ISSUES];
    size_t issue_count;
    Developer developers[IF_MAX_DEVELOPERS];
    size_t developer_count;
} RepositoryData;

typedef struct {
    char messages[IF_MAX_ERRORS][IF_TEXT];
    size_t count;
} ValidationErrors;

int if_parse_csv_row(const char *line, char fields[][IF_TEXT],
                     size_t max_fields, size_t *field_count);

int if_load_repository(const char *issues_path, const char *developers_path,
                       RepositoryData *data, char *error, size_t error_size);

int if_validate_repository(const RepositoryData *data, ValidationErrors *errors);

#endif
