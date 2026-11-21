# Open Source Issue Triage Tool

A Design and Analysis of Algorithms (DAA) project implemented in **C11**.

The application loads software issues and developer records from CSV files, validates the data, detects circular dependencies, sorts and searches issues, and assigns eligible issues to developers through a command-line interface.

## Algorithms used in the actual workflow

| Algorithm | Where it is used | Purpose |
|---|---|---|
| Merge Sort | Issue sorting and assignment order | Sort issue records by ID, priority, severity, deadline or estimated effort |
| Binary Search | Issue lookup | Find an issue by ID after sorting records by ID |
| Depth First Search (DFS) | Repository validation | Detect circular dependencies between issues |
| Greedy assignment | Developer assignment | Select an eligible developer using skill match and remaining capacity |

Only algorithms that solve a real issue-management task are included. Standalone algorithm demonstrations have been removed.

## Features

- C structures for Issue, Developer and RepositoryData.
- CSV parsing with quoted fields, commas and escaped double quotes.
- Validation of issue and developer records.
- Dependency checks for missing, repeated and self-references.
- DFS-based circular dependency detection.
- Stable Merge Sort for real issue records.
- Binary Search for issue lookup by ID.
- Greedy developer assignment based on matching skills and available hours.
- Assignment results and updated developer capacity in the CLI.
- Automated tests for loading, validation, cycle detection, sorting, searching and assignment.

## Greedy assignment rules

1. Issues are processed from highest to lowest priority using Merge Sort.
2. Only issues with status `open` or `in_progress` are eligible. `blocked` and `closed` issues are skipped.
3. A developer is eligible when at least one of their skills matches the issue module or one of its labels (case-insensitive exact match).
4. The developer must have enough remaining hours for the issue.
5. The algorithm chooses the developer with the most matching skills. Ties are broken by greater remaining availability, then greater experience.
6. The selected developer's available hours are reduced by the issue estimate.
7. If nobody qualifies, the issue remains unassigned.

This is a greedy heuristic. It makes the best local choice at each step and does not guarantee a globally optimal assignment. Existing assignments are retained if the assignment action is run again, preventing duplicate hour deductions.

## Build and run

Requirements: GCC or Clang with C11 support and CMake 3.16+.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/issueforge
```

Run the application from the repository root so the sample CSV paths resolve. Select option 1 to load data before viewing, validating, sorting, searching or assigning.

## CLI menu

1. Load sample repository data
2. View issues
3. View developers
4. Validate repository (includes DFS cycle detection)
5. Sort real issues (Merge Sort)
6. Search issue by ID (Binary Search)
7. Assign issues to developers (Greedy Algorithm)
8. View assignment results
0. Exit

## CSV format

Issues: `id,title,status,priority,module,hours,severity,deadline,labels,dependencies`

Developers: `id,name,skills,available_hours,experience`

Labels, skills and dependency IDs are semicolon-separated. Quote CSV fields containing commas; escape embedded double quotes by doubling them.

## Project structure

```text
include/issueforge/issueforge.h       Data structures and core API
include/issueforge/issue_workflow.h   Issue sorting/search API
include/issueforge/assignment.h      Greedy assignment API
src/csv.c                             CSV parsing and loading
src/validation.c                      Field validation and DFS cycle detection
src/issue_workflow.c                  Merge Sort and Binary Search on issues
src/assignment.c                      Greedy developer assignment
src/main.c                            Command-line interface
tests/test_foundation.c               CSV, validation and DFS tests
tests/test_issue_workflow.c           Integrated sorting/search tests
tests/test_assignment.c               Greedy assignment tests
docs/architecture.md                  Architecture and dependency graph
docs/phase2-algorithms.md              Integrated algorithm explanations
docs/phase3-assignment.md              Greedy assignment explanation
data/sample/                          Sample CSV datasets
```

## Complexity

Let n be the number of issues and d the number of developers. Merge Sort takes O(n log n). For each eligible issue, the greedy assignment scans developers and their skills/issue labels, so assignment selection is O(n * d * s * l) in the worst case, where s is the maximum skills per developer and l is the maximum labels per issue. The current limits make these scans small and predictable.

Binary Search takes O(log n) after sorting by ID. DFS detects dependency cycles; this implementation resolves dependency IDs by scanning the issue array, so its worst-case time is O(VE).

The sample dataset is synthetic and intended for demonstration.
