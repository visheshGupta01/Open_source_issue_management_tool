#include "issueforge/issueforge.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    char fields[8][IF_TEXT];
    size_t count = 0;

    assert(if_parse_csv_row("7,\"Fix, login\",open,high",
                            fields, 8, &count) == 0);
    assert(count == 4);
    assert(strcmp(fields[1], "Fix, login") == 0);

    assert(if_parse_csv_row("1,\"A \"\"quoted\"\" title\",open",
                            fields, 8, &count) == 0);
    assert(strcmp(fields[1], "A \"quoted\" title") == 0);

    RepositoryData data;
    ValidationErrors errors;
    char error[IF_TEXT];

    assert(if_load_repository("data/sample/issues.csv",
                              "data/sample/developers.csv",
                              &data, error, sizeof(error)) == 0);
    assert(data.issue_count == 8);
    assert(data.developer_count == 4);
    assert(if_validate_repository(&data, &errors) == 0);

    data.issues[0].dependencies[data.issues[0].dependency_count++] = 9999;
    assert(if_validate_repository(&data, &errors) != 0);

    puts("C foundation tests passed.");
    return 0;
}
