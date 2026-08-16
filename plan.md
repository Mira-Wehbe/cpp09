# C++ Module 09  - Plan

## things that i should know

-  **Orthodox Canonical Form** - default ctor, copy ctor, copy assignment, destructor
-  **References** - `T&`, `const T&`, pass-by-reference vs pass-by-value
-  **const correctness** - const parameters, const methods, const objects
-  **Dynamic memory** - `new`/`delete`, ownership, deep vs shallow copy
-  **Exceptions** - `try`/`catch`/`throw`, custom exception classes (`std::exception`, `what()`)
-  **Templates** - function templates, why implementation stays in header
-  **Streams** - `std::ifstream`, `std::stringstream`, `std::getline`
-  **STL Containers overview** - what `vector`, `list`, `map`, `deque`, `stack` are, and their rough performance tradeoffs (insert/lookup/iterate)
-  **Iterators** - `begin()`/`end()`, `const_iterator`, basic invalidation rules

---

## Ex00 — Bitcoin Exchange

**Learn before starting:**
- `std::map` - how it sorted by key, and its `lower_bound()` / `upper_bound()` methods (needed to find "closest date ≤ target")
- File reading: `std::ifstream` + `std::getline`
- String parsing without `<algorithm>`: `std::stringstream`, `substr`, `find`
- String → number conversion: `std::stringstream` or `atof`/`strtod`-style approaches (careful: not `atoi` from `<cstdlib>` if forbidden - check your subject's exact restriction)
- Manual date validation logic (splitting "YYYY-MM-DD", checking valid month/day ranges, leap years if you want to be thorough)
- Exception handling for malformed lines/values

**Container to use:** `std::map<std::string, float>`

---

## Ex01 — RPN (Reverse Polish Notation)

**Learn before starting:**
- Stack-based expression evaluation algorithm (push numbers, pop-two-and-compute on operators)
- `std::stack` - `push()`, `pop()`, `top()`, `empty()`
- Tokenizing a string by whitespace (`std::istringstream >> token` loop)
- Error cases to handle: division by zero, too few operands, invalid tokens, leftover values at the end

**Container to use:** `std::stack` (different from ex00's container — reuse is forbidden)

---

## Ex02 — PmergeMe

**Learn before starting:**
- **Ford-Johnson (merge-insertion) sort** — this is the core algorithm, study it in this order:
  1. Standard merge sort (mental warm-up)
  2. Pairing elements + sorting each pair
  3. Recursively sorting the larger elements → builds the "main chain"
  4. **Jacobsthal numbers** — what they are, why they determine insertion order
  5. Binary insertion of remaining ("pend") elements into the main chain following Jacobsthal order
- Implementing the *same* algorithm twice, once per container (no generic templated shortcut recommended by subject)
- Timing code: `clock()` from `<ctime>` (C++98-safe; `<chrono>` is C++11, not allowed)
- Handling large inputs (3000+ ints) — watch for accidental O(n²) blowups

**Containers to use:** two different containers, neither used in ex00 or ex01 (e.g. `std::vector` + `std::deque`, or `std::vector` + `std::list`)

---
