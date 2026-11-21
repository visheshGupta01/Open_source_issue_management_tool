# Phase 3 — Greedy developer assignment

## Goal

Assign eligible software issues to developers using the issue and developer records already loaded from CSV.

## Inputs

- Issue module, labels, priority, status and estimated hours.
- Developer skills, remaining available hours and experience.

## Greedy strategy

1. Sort issues by priority using the existing stable Merge Sort (critical, high, medium, low).
2. Process issues in that order.
3. Skip issues whose status is `blocked` or `closed`. Only `open` and `in_progress` issues are eligible.
4. A developer is eligible when at least one developer skill exactly matches the issue module or one of its labels. Matching is case-insensitive.
5. The developer must have enough remaining available hours.
6. Choose the eligible developer with the greatest number of matching skills.
7. Break ties by greater remaining available hours, then greater experience.
8. Record the developer ID on the issue and deduct the issue's estimated hours from that developer's availability.
9. If no developer qualifies, leave the issue unassigned.

This is a greedy heuristic. It makes a locally best choice at each step and does not guarantee a globally optimal assignment.

## Data representation

Each `Issue` contains `assigned_developer_id`. A value of zero means the issue has not been assigned. Assignments are held in memory for the current program run; the original CSV files are not modified.

## Complexity

Let:
- n = number of issues,
- d = number of developers,
- s = maximum skills per developer,
- l = maximum labels per issue.

Sorting takes O(n log n). Candidate selection takes O(d × s × l) per issue in the worst case, so the total assignment time is O(n log n + n × d × s × l). The fixed-size project limits keep these scans small.

## Sample data outcome

With the repository's current sample CSV files:

- 5 issues are assigned.
- 2 eligible issues remain unassigned because no developer has a matching skill.
- 1 blocked issue is skipped.

The exact developer assignment follows the deterministic tie-breaking rules above.

## Tests

`tests/test_assignment.c` verifies the expected sample assignments, unassigned and blocked issues, remaining developer hours, and that running the assignment action a second time does not deduct hours again.
