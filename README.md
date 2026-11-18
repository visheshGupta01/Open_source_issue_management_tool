# Open Source Issue Triage Tool

A Design and Analysis of Algorithms project implemented in **C11**.

## Phase 1 — Foundation

- C structures for Issue, Developer and RepositoryData.
- CSV parser supporting quoted fields, commas and escaped double quotes.
- CSV loading for issue and developer datasets.
- Validation of IDs, required fields, priorities, statuses, estimated hours, severity and deadlines.
- Dependency reference, duplicate and self-dependency checks.
- DFS-based dependency-cycle detection.
- Menu-driven command-line interface.
- Automated foundation tests with CTest.

## Build and run

Requirements: GCC or Clang with C11 support and CMake 3.16+.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/issueforge
```

Run the program from the repository root so the sample CSV paths resolve. Select option 1 to load the sample data.

## CSV format

Issues: `id,title,status,priority,module,hours,severity,deadline,labels,dependencies`

Developers: `id,name,skills,available_hours,experience`

Labels, skills and dependency IDs are semicolon-separated. CSV fields containing commas should be quoted. Embedded double quotes are escaped by doubling them.

## Project structure

```text
include/issueforge/issueforge.h   Shared structs and function declarations
src/csv.c                        CSV parsing and repository loading
src/validation.c                 Data validation and DFS cycle detection
src/main.c                       Command-line interface
data/sample/                     Sample CSV datasets
tests/test_foundation.c          Foundation tests
docs/architecture.md             Architecture and design notes
```

## Roadmap

| Phase | Scope | Status |
|---|---|---|
| 1 | Data models, CSV loading, validation, dependency-cycle detection, CLI | Implemented in C11 |
| 2 | Sorting, searching and divide-and-conquer algorithms | Next — C |
| 3 | Greedy, graph and dynamic programming | Planned |
| 4 | Backtracking, branch and bound, flow | Planned |
| 5 | Number theory, string matching and benchmarking | Planned |
| 6 | Integration, evaluation and final documentation | Planned |

The included dataset is synthetic and intended for demonstration.
