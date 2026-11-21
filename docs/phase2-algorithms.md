# Phase 2 — Algorithms integrated into issue triage

This phase uses only algorithms that directly support the IssueForge workflow. There is no separate algorithm laboratory.

## 1. Merge Sort — issue prioritization

Merge Sort is used to sort the actual `Issue` structures.

The user can sort issues by:

- ID: ascending.
- Priority: critical, high, medium, low.
- Severity: highest numeric value first.
- Deadline: fewest days remaining first.
- Estimated effort: lowest hours first.

The implementation is stable, so issues with equal sort keys keep their original relative order.

**Complexity:** O(n log n) time and O(n) auxiliary space.

## 2. Binary Search — issue lookup

Binary Search finds an issue using its ID.

The issue list must first be sorted by ID in ascending order. The CLI does this using the integrated Merge Sort function before calling Binary Search.

The search returns the zero-based array index when found, or -1 when the ID does not exist.

**Complexity:** O(log n) time and O(1) auxiliary space. Sorting before searching costs O(n log n); repeated searches can reuse the sorted order until the issue list changes.

## 3. Depth First Search — dependency cycle detection

Each issue is a vertex. If issue A depends on issue B, the dependency is represented as a directed edge from A to B.

DFS explores dependency paths and uses three states:

- 0: not visited.
- 1: currently being explored.
- 2: completely explored.

If DFS reaches a vertex in state 1, it has found a cycle. A cycle means a group of issues is waiting on one another and cannot be resolved in dependency order.

The DFS traversal itself is O(V + E) with adjacency lists. In this implementation, dependency IDs are resolved by scanning the issue array, which adds O(V) lookup work per dependency edge. Therefore, the full dependency traversal can take O(VE) time in the worst case.

## CLI workflow

1. Load the sample repository data.
2. View issues or developers.
3. Run repository validation to check fields, references and dependency cycles.
4. Sort actual issue records using Merge Sort.
5. Search for an issue by ID using Binary Search.

## Tests

The test suites cover:

- CSV parsing and repository loading.
- Required fields, duplicate IDs and dependency validation.
- Circular dependency detection through DFS.
- Sorting issue records by priority, deadline and ID.
- Successful and unsuccessful issue lookup.

Run all tests:

```bash
ctest --test-dir build --output-on-failure
```
