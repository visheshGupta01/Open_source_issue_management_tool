# Open Source Issue Triage Tool

A Design and Analysis of Algorithms (DAA) project implemented in **C11**.

The application loads software issues and developer records from CSV files, validates the data, detects circular dependencies, and provides issue sorting and searching through a command-line interface.

## Algorithms used in the actual workflow

| Algorithm | Where it is used | Purpose |
|---|---|---|
| Merge Sort | Issue sorting | Sort issue records by ID, priority, severity, deadline or estimated effort |
| Binary Search | Issue lookup | Find an issue by ID after sorting records by ID |
| Depth First Search (DFS) | Repository validation | Detect circular dependencies between issues |

Only algorithms that solve a real issue-management task are included. Standalone algorithm demonstrations have been removed.

### How the algorithms work together

1. The program loads issues and developers from CSV files.
2. Validation checks required fields, IDs, priorities, statuses and dependency references.
3. DFS checks the dependency graph for cycles.
4. Merge Sort orders the real issue records using the selected criterion.
5. Binary Search locates an issue by ID in the list sorted by ascending ID.

## Features

- C structures for Issue, Developer and RepositoryData.
- CSV parsing with quoted fields, commas and escaped double quotes.
- Validation of issue and developer records.
- Dependency checks for missing, repeated and self-references.
- DFS-based circular dependency detection.
- Stable Merge Sort for real issue records.
- Binary Search for issue lookup by ID.
- Automated tests for loading, validation, cycle detection, sorting and searching.

## Build and run

Requirements: GCC or Clang with C11 support and CMake 3.16+.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/issueforge
```

Run the application from the repository root so the sample CSV paths resolve. Select option 1 to load data before viewing, validating, sorting or searching.

## CLI menu

1. Load sample repository data
2. View issues
3. View developers
4. Validate repository (includes DFS cycle detection)
5. Sort real issues (Merge Sort)
6. Search issue by ID (Binary Search)
0. Exit

## CSV format

Issues: `id,title,status,priority,module,hours,severity,deadline,labels,dependencies`

Developers: `id,name,skills,available_hours,experience`

Labels, skills and dependency IDs are semicolon-separated. Quote CSV fields containing commas; escape embedded double quotes by doubling them.

## Project structure

```text
include/issueforge/issueforge.h       Data structures and core API
include/issueforge/issue_workflow.h   Issue sorting/search API
src/csv.c                             CSV parsing and loading
src/validation.c                      Field validation and DFS cycle detection
src/issue_workflow.c                  Merge Sort and Binary Search on issues
src/main.c                            Command-line interface
tests/test_foundation.c               CSV, validation and DFS tests
tests/test_issue_workflow.c           Integrated sorting/search tests
docs/architecture.md                  Architecture and dependency graph
docs/phase2-algorithms.md              Integrated algorithm explanations
data/sample/                          Sample CSV datasets
```

## Complexity

- Merge Sort: O(n log n) time and O(n) auxiliary space.
- Binary Search: O(log n) time and O(1) auxiliary space.
- DFS traversal: O(V + E) when the graph is represented with adjacency lists. This implementation resolves dependency IDs by scanning the issue array, so dependency lookup adds linear work per edge.

The sample dataset is synthetic and intended for demonstration.
