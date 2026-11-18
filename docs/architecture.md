# Architecture — Phase 1 (C11)

IssueForge is organized as a small modular C application. Public data structures and function declarations live in `include/issueforge/issueforge.h`.

- **Data model:** fixed-capacity C structs for issues, developers and repository data.
- **CSV layer:** parsing and loading are implemented in `src/csv.c`.
- **Validation layer:** field validation, dependency reference checks and cycle detection are implemented in `src/validation.c`.
- **CLI:** menu-driven interaction is implemented in `src/main.c`.

## Dependency graph

An issue's dependency list stores IDs of issues that must be completed first. The validator checks that referenced issues exist and applies depth-first search with three states:

1. 0 — unvisited
2. 1 — currently being explored
3. 2 — fully explored

An edge to a node in state 1 indicates a directed cycle.

## C design decisions

- The project uses C11; there are no C++ source files or libraries.
- Fixed-size arrays keep memory ownership straightforward for this introductory DAA project.
- Capacity limits are declared in the public header.
- Functions return status codes and write error messages into caller-provided buffers.
- Phase 2 will add sorting and searching as standalone C functions, with tests and complexity analysis.
