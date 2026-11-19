# Open Source Issue Triage Tool

A Design and Analysis of Algorithms project implemented in **C11**.

## Phase 1 — Foundation
- C structures for Issue, Developer and RepositoryData.
- CSV parser and loading for issue/developer datasets.
- Field and dependency validation, including DFS dependency-cycle detection.
- Menu-driven CLI and automated CTest coverage.

## Phase 2 — Algorithms applied to issue triage
- **Merge Sort** orders the actual issue records by ID, priority, severity, deadline or estimated effort.
- **Binary Search** finds an issue by ID in the issue list after it is sorted by ID.
- The algorithm laboratory retains demonstrations and tests for Quick Sort, recursive/iterative binary search, maximum subarray, fast exponentiation, celebrity identification, and classical/Strassen matrix multiplication.
- Integration tests load the sample CSV and verify sorting and lookup against real project records.

The practical workflow is available from CLI options 6 and 7. Option 5 remains the standalone algorithm laboratory.

## Build and run
Requirements: GCC or Clang with C11 support and CMake 3.16+.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/issueforge
```

Run from the repository root so sample CSV paths resolve. Select option 1 to load sample data before viewing, sorting or searching issues.

## CSV format
Issues: id,title,status,priority,module,hours,severity,deadline,labels,dependencies

Developers: id,name,skills,available_hours,experience

Labels, skills and dependency IDs are semicolon-separated. Quote CSV fields containing commas; escape embedded double quotes by doubling them.

## Project structure
```text
include/issueforge/issueforge.h       Phase 1 domain API
include/issueforge/algorithms.h       Phase 2 algorithm API
include/issueforge/issue_workflow.h  Issue sorting/search API
src/csv.c                             CSV parsing and loading
src/validation.c                      Data validation and DFS cycle detection
src/algorithms_sort_search.c          Generic sorting and searching
src/algorithms_divide_conquer.c       Maximum subarray, power and celebrity
src/algorithms_matrix.c               Classical and Strassen multiplication
src/issue_workflow.c                  Merge Sort and Binary Search on Issue records
src/main.c                            CLI and demonstrations
tests/test_foundation.c               Phase 1 tests
tests/test_algorithms.c               Phase 2 algorithm tests
tests/test_issue_workflow.c           Real-data integration tests
docs/phase2-algorithms.md              Complexity and design notes
data/sample/                          Synthetic sample CSV datasets
```

## Roadmap
| Phase | Scope | Status |
|---|---|---|
| 1 | Data models, CSV loading, validation and dependency-cycle detection | Implemented |
| 2 | Algorithm laboratory plus issue sorting and searching integration | Implemented |
| 3 | Greedy, graph and dynamic programming | Planned |
| 4 | Backtracking, branch and bound, flow | Planned |
| 5 | Number theory and string matching | Planned |
| 6 | Integration, evaluation and final documentation | Planned |

The included dataset is synthetic and intended for demonstration. See docs/phase2-algorithms.md for complexity analysis and implementation notes.
