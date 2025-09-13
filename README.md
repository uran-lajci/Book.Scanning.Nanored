# Book.Scanning.Nanored

The solutions for the Book Scanning Problem in this repository are taken from this repository: https://github.com/Nanored4498/Hash2020/tree/master.

We have:
- organized files in a structured format
- added batch scripts for automatical code execution
- added a evaluation script to get the scores of the solutions
- tested the solutions against a bigger dataset

Important note: The original code of the authors has not been changed.

## Solutions approaches

### Adaptive Greedy (b_read_on and f_libraries_of_the_world tuned)

This solution uses a **greedy, dynamically re-evaluated strategy** to decide which libraries to sign up and which books to scan before time runs out.

At the start, it **sorts each library’s books by score**, and builds quick-lookup structures mapping each book to the libraries that have it and their positions within those libraries. This allows fast updates when books get claimed.

It then repeatedly **picks the best next library** using a score that balances:

* the total value of its highest-scoring remaining books (that can fit in the remaining days),
* a penalty for long signup time, and
* a small penalty for having very uneven book scores.

Before committing to a library, it **recomputes its score ignoring already-taken books** and ensures the chosen books can be scanned in the available days. Once a library is selected, its chosen books are marked as taken, and their scores are **deducted from the estimated values of other libraries** that also contained them. The current day is then advanced by that library’s signup time, and the process repeats.

This **adaptive greedy approach** lets the algorithm prioritize high-yield libraries while dynamically adjusting as books are removed from the pool.

### Fast Greedy with Lightweight Re-scoring (c_incunabula tuned)

It first computes a priority for each library:
* Base value: sum of scores of all its books
* Time penalty: subtract 0.09*mean_book_score * T[i]
* Normalization: divide by (ADD + T[i]^POW) with ADD=90, POW=1.0

It pushes (value, library) into a max-heap. While there’s time left, it pops the best library that can finish signup with at least one scanning day, adds all currently unscanned books from it (in given order), marks them taken, and penalizes every other library containing each taken book by subtracting S[b]/(ADD + T[l]^POW) from their priority (lazy-updating the heap). Time advances by that library’s signup time. Finally, it outputs the signup order and book lists, then simulates the actual schedule (respecting per-day throughput M[i] and horizon D) to print a true score.
Trade-offs: simple, fast, and adaptive to duplicates; it ignores M[i] during selection (only checked later in simulation), so chosen lists can overshoot the scan capacity, but the greedy penalties help steer choices toward higher-yield, faster libraries.

### Degree-based Greedy (d_tough_choices tuned)

This is a **degree-based greedy** picker that ignores book scores and most timing during selection, then simulates the real schedule at the end. It builds a max-heap key `N[i]` per library, initialized as `MUL * (original book count)` and **boosted** by `ADD` for each book that appears in **exactly two** libraries (favoring “shared but not too common” books). While `t < D`, it repeatedly pops the current best library, adds **all unseen books** from it, and for every taken book decrements the heap key of all libraries that also had that book (−`MUL`, and an extra −`ADD` if that book was in exactly two libraries), then lazily re-pushes updates. The loop advances time by a **fixed 2-day signup** and only does a crude feasibility check `t + 2 + |picked| < D` (so it **doesn’t use real `T[i]` or `M[i]` during selection**, nor book scores `S[b]`). After producing the plan, it **simulates with actual `T[i]` and `M[i]`** to compute the true score.
**Upshot:** very fast and adaptive to duplicates via local penalties, but heuristic—may over-pick slow/low-throughput libraries or low-value books since it ignores `S`, real signup times, and per-day limits during selection.

### Adaptive greedy with on-the-fly re-scoring (e_so_many_books tuned)

For each library, sort its books by score and estimate its initial value as the sum of the top `n = min(N[i], M[i]*(D−T[i]))` books, then normalize by a time term:
`value = (sum_top_n − ratio*T[i]) / (DC*mean_T + T[i]^D_POW)` with `ratio ≈ TIME_MUL * (mean_value/mean_T)`.
All libraries go in a max-heap. While days remain, pop the best candidate and **recompute** its value for the *current* time: slide through its sorted list, skipping already-taken books and extending the window so you still get `n` unique books; apply a **variance penalty** (`−65·stddev`) so libraries with a few spikes are down-weighted. If still best, select the library, then iterate its books: for each unseen book, mark it taken, **penalize other libraries that counted it in their current top window**, and add it to this library’s plan only if `t + T[lib] + |picked|/M[lib] < D`. Advance time by `T[lib]` and repeat.
**Pros:** fast, adaptive, accounts for signup time and throughput in both scoring and feasibility; avoids duplicate waste via localized score deductions. **Cons:** heuristic (tuned constants `TIME_MUL, D_POW, DC`, fixed variance weight), so quality depends on instance characteristics.

_Note:_ ChatGPT 5 was used to get the analysis of the variants.
