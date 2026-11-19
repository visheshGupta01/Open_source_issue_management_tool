# Phase 2 — Algorithms and issue-triage integration

Phase 2 contains both a reusable algorithm laboratory and algorithms connected to actual IssueForge data.

## Algorithms integrated into the product

| Algorithm | Project feature | Complexity |
|---|---|---:|
| Merge Sort | Sort issue records by ID, priority, severity, deadline or estimated effort | O(n log n) time, O(n) extra space |
| Binary Search | Find an issue by ID after sorting records by ascending ID | O(log n) time, O(1) extra space |

### Issue sorting rules

- Priority: critical, high, medium, low, then unrecognized values.
- Severity: highest numeric severity first.
- Deadline: fewest days remaining first.
- Estimated effort: lowest estimated hours first.
- ID: ascending numeric order.

The sort is stable: records with equal sort keys keep their relative input order. Sorting changes the in-memory issue array order but does not modify the CSV file.

### Issue lookup

The CLI's search action first sorts the in-memory records by ID with Merge Sort, then uses iterative Binary Search. The search API itself expects an array already sorted by ascending ID and returns a zero-based index or -1.

## Other Phase 2 algorithms

| Algorithm | Purpose | Time complexity | Extra space |
|---|---|---:|---:|
| Quick Sort | In-place integer sorting demonstration | Average O(n log n), worst O(n²) | O(log n) typical stack |
| Recursive Binary Search | Recursive sorted-data search demonstration | O(log n) | O(log n) |
| Maximum subarray | Best contiguous interval in a numeric sequence | O(n log n) | O(log n) |
| Fast exponentiation | Compute a^b by squaring | O(log |b|) | O(1) |
| Celebrity problem | Identify a person known by everyone who knows nobody | O(n) | O(1) |
| Classical matrix multiplication | Square matrix product | O(n³) | O(1) auxiliary |
| Strassen multiplication | Divide-and-conquer matrix product | O(n^log₂7) asymptotically | O(n²) auxiliary per active level |

These remaining algorithms are retained as DAA demonstrations and tests; they are not forced into the issue-triage workflow where their use would be artificial.

## CLI demonstration

1. Load sample repository data.
2. Choose **Sort real issues** and select a criterion.
3. Choose **Search issue by ID** and enter an ID such as 106.
4. Try an unknown ID such as 999 to see the not-found result.

## Tests

The issue workflow test loads the sample CSV and verifies priority ordering, deadline ordering, ID ordering, successful lookup, missing lookup and invalid input handling. Run all suites with:

```bash
ctest --test-dir build --output-on-failure
```

The algorithm laboratory also covers sorting, both search variants, maximum-subarray edge cases, positive and negative powers, celebrity detection, and agreement between classical and Strassen multiplication on a non-power-of-two matrix.
