# Architecture — IssueForge (C11)

IssueForge is a modular command-line application. Public data structures and function declarations are stored in `include/issueforge/`.

- **Data model:** fixed-capacity C structures for issues, developers and repository data.
- **CSV layer:** `src/csv.c` parses and loads issue and developer records.
- **Validation layer:** `src/validation.c` checks fields, IDs, dependency references and circular dependencies using DFS.
- **Issue workflow:** `src/issue_workflow.c` applies stable Merge Sort to real issue records and Binary Search to find issues by ID.
- **CLI:** `src/main.c` connects user menu choices to loading, validation, sorting and searching.

## Algorithms in the application

### Merge Sort

Sorts issue records by ID, priority, severity, deadline or estimated effort. It uses divide and conquer and has O(n log n) time complexity and O(n) auxiliary space.

### Binary Search

Finds an issue by ID in an array sorted by ascending ID. It repeatedly halves the search interval and takes O(log n) time.

### Depth First Search

Treats issues as vertices and dependencies as directed edges. It detects a cycle when it reaches a vertex that is currently being explored. The DFS traversal is O(V + E) with adjacency lists; this implementation uses linear issue-ID lookup for dependency edges.

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
