# Open Source Issue Triage Tool

A Design and Analysis of Algorithms project implemented in **C11**.

## Phase 1 — Foundation

- C structures for Issue, Developer and RepositoryData.
- CSV parser supporting quoted fields, commas and escaped double quotes.
- CSV loading for issue and developer datasets.
- Field and dependency validation, including DFS-based dependency-cycle detection.
- Menu-driven command-line interface and automated CTest coverage.

## Phase 2 — Algorithm Laboratory

- Merge sort and quick sort for integer arrays.
- Iterative and recursive binary search.
- Divide-and-conquer maximum contiguous subarray.
- Fast exponentiation by squaring, including negative exponents.
- Linear-time celebrity identification.
- Classical and Strassen square matrix multiplication; Strassen supports dimensions that are not powers of two through zero-padding.
- Dedicated automated tests and algorithm complexity notes.

Choose option 5 in the CLI to run the Phase 2 demonstrations. The algorithm functions are also available independently through include/issueforge/algorithms.h.

## Build and run

Requirements: GCC or Clang with C11 support and CMake 3.16+.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/issueforge
```

Run from the repository root so sample CSV paths resolve. Select option 1 to load sample repository data.

## CSV format

Issues: id,title,status,priority,module,hours,severity,deadline,labels,dependencies

Developers: id,name,skills,available_hours,experience

Labels, skills and dependency IDs are semicolon-separated. CSV fields containing commas should be quoted; embedded double quotes are escaped by doubling them.

## Project structure

```text
include/issueforge/issueforge.h       Phase 1 domain models and API
include/issueforge/algorithms.h       Phase 2 algorithm API
src/csv.c                             CSV parsing and repository loading
src/validation.c                      Data validation and dependency-cycle detection
src/algorithms_sort_search.c          Sorting and searching
src/algorithms_divide_conquer.c       Maximum subarray, power, celebrity
src/algorithms_matrix.c               Classical and Strassen multiplication
src/main.c                            Command-line interface and demos
tests/test_foundation.c               Phase 1 tests
tests/test_algorithms.c               Phase 2 tests
docs/phase2-algorithms.md              Complexity and design notes
data/sample/                          Synthetic sample CSV datasets
```

## Roadmap

| Phase | Scope | Status |
|---|---|---|
| 1 | Data models, CSV loading, validation and dependency-cycle detection | Implemented |
| 2 | Sorting, searching and divide-and-conquer algorithm laboratory | Implemented |
| 3 | Greedy, graph and dynamic programming | Planned |
| 4 | Backtracking, branch and bound, flow | Planned |
| 5 | Number theory and string matching | Planned |
| 6 | Integration, evaluation and final documentation | Planned |

The included dataset is synthetic and intended for demonstration. See docs/phase2-algorithms.md for complexity analysis and implementation notes.
