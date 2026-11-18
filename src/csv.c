#include "issueforge/issueforge.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int if_parse_csv_row(const char *line, char fields[][IF_TEXT],
                     size_t max_fields, size_t *field_count) {
    size_t i = 0, col = 0, pos = 0;
    int quoted = 0;

    if (!line || !fields || !field_count || max_fields == 0) return -1;
    memset(fields, 0, max_fields * IF_TEXT);

    while (line[i] && line[i] != '\n' && line[i] != '\r') {
        char ch = line[i++];

        if (quoted) {
            if (ch == '"') {
                if (line[i] == '"') {
                    ch = '"';
                    ++i;
                } else {
                    quoted = 0;
                    continue;
                }
            }
        } else {
            if (ch == ',') {
                if (col + 1 >= max_fields) return -1;
                ++col;
                pos = 0;
                continue;
            }
            if (ch == '"' && pos == 0) {
                quoted = 1;
                continue;
            }
        }

        if (pos + 1 >= IF_TEXT) return -1;
        fields[col][pos++] = ch;
    }

    if (quoted) return -1;
    *field_count = col + 1;
    return 0;
}

static int parse_int(const char *s, int *out) {
    char *end = NULL;
    long value;

    while (isspace((unsigned char)*s)) ++s;
    if (!*s) return -1;

    value = strtol(s, &end, 10);
    while (end && isspace((unsigned char)*end)) ++end;

    if (!end || *end || value < -2147483647L - 1 || value > 2147483647L)
        return -1;

    *out = (int)value;
    return 0;
}

static size_t split_list(const char *s, char out[][64], size_t capacity) {
    char temp[IF_TEXT];
    size_t count = 0;
    char *save = NULL;

    snprintf(temp, sizeof(temp), "%s", s);
    for (char *part = strtok_r(temp, ";", &save);
         part && count < capacity;
         part = strtok_r(NULL, ";", &save)) {
        size_t len;
        while (*part == ' ') ++part;
        len = strlen(part);
        while (len && part[len - 1] == ' ') part[--len] = '\0';
        if (len) {
            snprintf(out[count], 64, "%s", part);
            ++count;
        }
    }
    return count;
}

static int split_ids(const char *s, int out[], size_t capacity, size_t *count) {
    char temp[IF_TEXT];
    char *save = NULL;

    snprintf(temp, sizeof(temp), "%s", s);
    *count = 0;

    for (char *part = strtok_r(temp, ";", &save);
         part;
         part = strtok_r(NULL, ";", &save)) {
        if (*count >= capacity || parse_int(part, &out[*count]) != 0)
            return -1;
        ++(*count);
    }
    return 0;
}

static int read_issues(const char *path, RepositoryData *data,
                       char *error, size_t error_size) {
    FILE *fp = fopen(path, "r");
    char line[4096];
    char fields[16][IF_TEXT];
    size_t count, line_no = 1;

    if (!fp) {
        snprintf(error, error_size, "Cannot open issues CSV: %s", path);
        return -1;
    }
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        snprintf(error, error_size, "Issues CSV is empty");
        return -1;
    }

    while (fgets(line, sizeof(line), fp)) {
        Issue issue = {0};
        ++line_no;
        if (line[0] == '\n' || line[0] == '\r') continue;

        if (data->issue_count >= IF_MAX_ISSUES ||
            if_parse_csv_row(line, fields, 16, &count) != 0 || count != 10) {
            snprintf(error, error_size, "Invalid issues CSV row at line %zu", line_no);
            fclose(fp);
            return -1;
        }

        if (parse_int(fields[0], &issue.id) != 0 ||
            parse_int(fields[5], &issue.estimated_hours) != 0 ||
            parse_int(fields[6], &issue.severity) != 0 ||
            parse_int(fields[7], &issue.deadline_days) != 0 ||
            split_ids(fields[9], issue.dependencies, IF_LIST,
                      &issue.dependency_count) != 0) {
            snprintf(error, error_size, "Invalid numeric field at issues CSV line %zu", line_no);
            fclose(fp);
            return -1;
        }

        snprintf(issue.title, sizeof(issue.title), "%s", fields[1]);
        snprintf(issue.status, sizeof(issue.status), "%s", fields[2]);
        snprintf(issue.priority, sizeof(issue.priority), "%s", fields[3]);
        snprintf(issue.module, sizeof(issue.module), "%s", fields[4]);
        issue.label_count = split_list(fields[8], issue.labels, IF_LIST);
        data->issues[data->issue_count++] = issue;
    }

    fclose(fp);
    return 0;
}

static int read_developers(const char *path, RepositoryData *data,
                           char *error, size_t error_size) {
    FILE *fp = fopen(path, "r");
    char line[4096];
    char fields[8][IF_TEXT];
    size_t count, line_no = 1;

    if (!fp) {
        snprintf(error, error_size, "Cannot open developers CSV: %s", path);
        return -1;
    }
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        snprintf(error, error_size, "Developers CSV is empty");
        return -1;
    }

    while (fgets(line, sizeof(line), fp)) {
        Developer developer = {0};
        ++line_no;
        if (line[0] == '\n' || line[0] == '\r') continue;

        if (data->developer_count >= IF_MAX_DEVELOPERS ||
            if_parse_csv_row(line, fields, 8, &count) != 0 || count != 5 ||
            parse_int(fields[0], &developer.id) != 0 ||
            parse_int(fields[3], &developer.available_hours) != 0 ||
            parse_int(fields[4], &developer.experience_years) != 0) {
            snprintf(error, error_size, "Invalid developers CSV row at line %zu", line_no);
            fclose(fp);
            return -1;
        }

        snprintf(developer.name, sizeof(developer.name), "%s", fields[1]);
        developer.skill_count = split_list(fields[2], developer.skills, IF_LIST);
        data->developers[data->developer_count++] = developer;
    }

    fclose(fp);
    return 0;
}

int if_load_repository(const char *issues_path, const char *developers_path,
                       RepositoryData *data, char *error, size_t error_size) {
    if (!issues_path || !developers_path || !data || !error || error_size == 0)
        return -1;

    memset(data, 0, sizeof(*data));
    error[0] = '\0';

    if (read_issues(issues_path, data, error, error_size) != 0) return -1;
    if (read_developers(developers_path, data, error, error_size) != 0) return -1;
    return 0;
}
