# Phase 2 — Algorithm Laboratory

All algorithms are implemented manually in C11 and are independent of the command-line interface.

| Algorithm | Purpose | Time complexity | Extra space |
|---|---|---:|---:|
| Merge sort | Stable integer sorting | O(n log n) | O(n) |
| Quick sort | In-place integer sorting | Average O(n log n), worst O(n²) | O(log n) typical stack |
| Iterative binary search | Search sorted data | O(log n) | O(1) |
| Recursive binary search | Recursive sorted-data search | O(log n) | O(log n) |
| Maximum subarray | Best contiguous workload interval | O(n log n) | O(log n) |
| Fast exponentiation | Compute a^b by squaring | O(log |b|) | O(1) |
| Celebrity problem | Person known by everyone who knows nobody | O(n) | O(1) |
| Classical matrix multiplication | Square matrix product | O(n³) | O(1) auxiliary |
| Strassen multiplication | Divide-and-conquer matrix product | O(n^log₂7) asymptotically | O(n²) auxiliary per active level |

## Design notes

- Merge sort uses one temporary array and is stable.
- Quick sort uses a middle pivot and recurses into the smaller partition first.
- Binary search requires ascending input and returns a zero-based index or -1.
- Maximum subarray returns the sum and inclusive start/end indices; all-negative arrays are supported.
- Fast exponentiation supports negative exponents and rejects zero to a negative exponent. Results use double.
- Celebrity input is a row-major count-by-count byte matrix; nonzero means the row person knows the column person.
- Matrix functions use row-major square int64_t matrices. Strassen pads non-power-of-two dimensions and switches to classical multiplication at size 32 or less.
- Integer overflow is not checked; choose values whose intermediate sums and products fit in int64_t.

## Tests

The Phase 2 test target covers sorting, both search variants, maximum-subarray edge cases, positive and negative powers, celebrity detection, and agreement between classical and Strassen multiplication on a non-power-of-two matrix.
