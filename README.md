# CPP Module 09 — STL

_This project has been created as part of the 42 curriculum by miwehbe._

# Description

CPP Module 09 is the last module of the C++ piscine. The goal is to solve three practical problems using the containers of the Standard Template Library (STL), all written in **C++98**. Each exercise must use a container that suits the problem, and a container used in one exercise can't be reused in the next ones.

| Exercise | Program    | Container(s)                 | Problem                                           |
|----------|------------|------------------------------|---------------------------------------------------|
| ex00     | `btc`      | `std::map`                   | Bitcoin value on a given date from a price database |
| ex01     | `RPN`      | `std::stack`                 | Evaluate a Reverse Polish Notation expression      |
| ex02     | `PmergeMe` | `std::vector` + `std::deque` | Sort integers with the Ford-Johnson merge-insertion algorithm |

# Requirements

- C++98 compiler (c++ / g++ / clang++)
- POSIX-compliant system (Linux / macOS)
- `make`

# Instructions

After cloning the repository, enter the folder of the exercise you want, then build and run it.

```bash
make        # build the executable
make re     # clean rebuild
make clean  # remove object files
make fclean # remove object files and executable
```

All programs are compiled with `-Wall -Wextra -Werror -std=c++98`.

Or to show leaks:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./btc input.txt
valgrind --leak-check=full --show-leak-kinds=all ./RPN "1 2 +"
valgrind --leak-check=full --show-leak-kinds=all ./PmergeMe 3 5 9 7 4
```

# Architecture

```
.
├── ex00/
│   ├── main.cpp                 # Entry point, argument check
│   ├── BitcoinExchange.cpp/.hpp # Database loading, date/value validation, rate lookup
│   ├── data.csv                 # Bitcoin price database (date,exchange_rate)
│   ├── input.txt                # Example input file (date | value)
│   └── Makefile
├── ex01/
│   ├── main.cpp                 # Entry point, argument check
│   ├── RPN.cpp/.hpp             # Tokenizer + stack-based evaluator
│   └── Makefile
└── ex02/
    ├── main.cpp                 # Entry point, timing, Before/After output
    ├── PmergeMe.cpp/.hpp        # Input parsing, Jacobsthal order, Ford-Johnson (vector & deque)
    └── Makefile
```

# ex00 — Bitcoin Exchange

The program reads a price database (`data.csv`) and an input file, then prints the value of a given amount of bitcoin on each date.

### How it works

- `data.csv` is loaded into a `std::map<std::string, float>` (date → rate). Since the map is sorted by key and dates are in `YYYY-MM-DD` format, string order is also chronological order.
- Each line of the input file must follow the format `date | value`.
- The **date** is validated: exact `YYYY-MM-DD` format, month between 1 and 12, and day valid for that month (leap years handled for February).
- The **value** must be a valid number (int or float) between `0` and `1000`.
- If the date is not in the database, the program uses the **closest lower date** (found with `std::map::lower_bound`, then stepping back one element).

### Usage

```bash
./btc input.txt
```

Example `input.txt`:

```
date | value
2011-01-03 | 3
2011-01-03 | 2
2011-01-03 | 1
2011-01-03 | 1.2
2011-01-09 | 1
2012-01-11 |-1
2001-42-42
2012-01-11 | 1
2012-01-11 | 2147483648
```

Output:

```
2011-01-03 => 3 = 0.9
2011-01-03 => 2 = 0.6
2011-01-03 => 1 = 0.3
2011-01-03 => 1.2 = 0.36
2011-01-09 => 1 = 0.32
Error: not a positive number.
Error: bad input => 2001-42-42
2012-01-11 => 1 = 7.1
Error: too large a number.
```

### Errors handled

| Case                         | Message                          |
|------------------------------|----------------------------------|
| Wrong number of arguments    | `Error: could not open file.`    |
| File can't be opened         | `Error: could not open file`     |
| Missing `\|` or invalid date | `Error: bad input => <line>`     |
| Negative value               | `Error: not a positive number.`  |
| Value above 1000             | `Error: too large a number.`     |
| Date before the first entry  | `Error: date is too early`       |

# ex01 — Reverse Polish Notation

The program takes a mathematical expression in Reverse Polish Notation as a single argument and prints the result.

### How it works

- The expression is split into tokens separated by spaces.
- A **number** (single digit `0`–`9`) is pushed onto a `std::stack<int>`.
- An **operator** (`+ - * /`) pops the top two numbers, applies the operation (`first op second`), and pushes the result back.
- At the end, exactly one number must remain on the stack: that's the result.

### Usage

```bash
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
42
./RPN "7 7 * 7 -"
42
./RPN "1 2 * 2 / 2 * 2 4 - +"
0
./RPN "(1 + 1)"
Error
```

### Errors handled

Every error prints `Error` on the standard error:

- Wrong number of arguments
- Invalid token (parentheses, numbers ≥ 10, letters, decimals…)
- Not enough numbers on the stack for an operator
- Division by zero
- More than one number left at the end

# ex02 — PmergeMe

The program takes a sequence of positive integers as arguments and sorts it using the **Ford-Johnson algorithm** (merge-insertion sort), once with `std::vector` and once with `std::deque`, then compares the time taken by each.

### How it works

1. **Pair** the elements two by two and put the smaller one first in each pair. If the count is odd, the last element is kept aside (the *stray*).
2. Take the **larger** element of each pair and sort them **recursively** with the same algorithm.
3. Build the **main chain** from the sorted larger elements, and put the smaller partner of the first one at the front (it's always smaller).
4. **Insert the remaining smaller elements** with binary search (`std::lower_bound`), following the **Jacobsthal order** (`1, 3, 5, 11, 21, 43…`, where `J(n) = J(n-1) + 2 * J(n-2)`). Inside each group they are inserted in reverse (e.g. `3, 2`, then `5, 4`, then `11 … 6`), which keeps the number of comparisons low.
5. Insert the **stray** element, if there is one.

### Usage

```bash
./PmergeMe 3 5 9 7 4
Before: 3 5 9 7 4
After: 3 4 5 7 9
Time to process a range of 5 elements with std::vector : 12 us
Time to process a range of 5 elements with std::deque : 15 us
```

Test with a large random sequence:

```bash
# Linux
./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "`

# macOS
./PmergeMe `jot -r 3000 1 100000 | tr '\n' ' '`
```

The timing includes both filling the container and sorting it.

### Errors handled

| Case                          | Message                               |
|-------------------------------|---------------------------------------|
| No arguments                  | `Error: no arguments`                 |
| Empty argument                | `Error: empty argument`               |
| Only a `+` sign               | `Error: malformed argument => <arg>`  |
| Non-numeric / negative input  | `Error: non numeric argument => <arg>`|
| `0` or value above `INT_MAX`  | `Error: value out of range => <arg>`  |

# Resources

STL containers:
- https://cplusplus.com/reference/stl/
- https://en.cppreference.com/w/cpp/container

std::map and lower_bound:
- https://cplusplus.com/reference/map/map/lower_bound/

Reverse Polish Notation:
- https://en.wikipedia.org/wiki/Reverse_Polish_notation

Ford-Johnson / merge-insertion sort:
- https://en.wikipedia.org/wiki/Merge-insertion_sort

Jacobsthal numbers:
- https://en.wikipedia.org/wiki/Jacobsthal_number

# AI Usage

- Clarifying missing pieces
- Testing edge cases
- Understanding the Ford-Johnson algorithm and the Jacobsthal order
- ReadMe formatting and fixing
