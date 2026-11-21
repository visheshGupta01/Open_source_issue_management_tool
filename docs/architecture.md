# Architecture — IssueForge (C11)

IssueForge is a modular command-line application. Public data structures and function declarations are stored in `include/issueforge/`.

- **Data model:** fixed-capacity C structures for issues, developers and repository data. Each issue stores its assigned developer ID; zero means unassigned.
- **CSV layer:** `src/csv.c` parses and loads issue and developer records.
- **Validation layer:** `src/validation.c` checks fields, IDs, dependency references and circular dependencies using DFS.
- **Issue workflow:** `src/issue_workflow.c` applies stable Merge Sort to real issue records and Binary Search to find issues by ID.
- **Assignment workflow:** `src/assignment.c` uses Merge Sort to process issues by priority, then greedily selects a developer based on skill overlap and remaining capacity.
- **CLI:** `src/main.c` connects user menu choices to loading, validation, sorting, searching and assignment.

## Algorithms in the application

### Merge Sort

Sorts issue records by ID, priority, severity, deadline or estimated effort. It is also used to process higher-priority issues first during assignment. It uses O(n log n) time and O(n) auxiliary space.

### Binary Search

Finds an issue by ID in an array sorted by ascending ID. It repeatedly halves the search interval and takes O(log n) time.

### Depth First Search

Treats issues as vertices and dependencies as directed edges. It detects a cycle when it reaches a vertex that is currently being explored. The implementation resolves dependency IDs with a linear scan, so the overall worst-case time is O(VE).

### Greedy developer assignment

For each eligible issue in priority order, the algorithm considers developers with enough remaining hours and at least one skill matching the issue module or labels. It selects the developer with the highest number of matching skills. Ties are broken by remaining available hours and then experience. The chosen developer's capacity is reduced immediately.

This is a local-choice heuristic and does not guarantee a globally optimal allocation.

## Dependency graph

An issue's dependency list stores IDs of issues that must be completed first. The validator checks that referenced issues exist, rejects self and repeated dependencies, and applies DFS with three states:

1. 0 — unvisited.
2. 1 — currently being explored.
3. 2 — fully explored.

An edge to a node in state 1 indicates a directed cycle.

## C design decisions

- The project uses the C11 standard and contains no C++ code.
- Fixed-size arrays keep memory ownership straightforward for this introductory project.
- Functions return status codes and write error messages into caller-provided buffers.
- Algorithms are integrated into actual issue-management operations; unrelated standalone demonstrations are excluded.
