![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)

# Matrix

## Table of Contents

- [How to Install](#how-to-install)
- [How to Build](#how-to-build)
- [Level 1 — Determinant](#level-1--determinant)
- [Level 2 — Matrix Chain Multiplication](#level-2--matrix-chain-multiplication)
- [Unit-tests](#unit-tests)
- [End-to-End Tests](#end-to-end-tests)

## How to Install

```bash
git clone https://github.com/daniilgriga/Matrix.git
cd Matrix/
```

## How to Build

```bash
# Debug version:
cmake -S . -B build/debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DSANITIZE=ON
cmake --build build/debug

# Release version:
cmake -S . -B build/release -G Ninja -DCMAKE_BUILD_TYPE=Release -DSANITIZE=OFF -DBUILD_TESTS=OFF
cmake --build build/release
```

## Level 1 — Determinant

Calculates the determinant of a square matrix.

CLI determinant mode reads floating-point input and uses the floating-point elimination path.
At class level (`Matrix<T>`), determinant uses:
- Bareiss for signed integral types
- Gaussian elimination for floating-point types

### Input Format

The program accepts input data through standard input (`stdin`) in the following format:

```
<matrix_size> <element_1> <element_2> ... <element_n²>
```

### Rules

1. **First number**: size of the square matrix `n` (n >= 1)
2. **Next n² numbers**: matrix elements in row-major order
3. **Elements**: floating-point numbers
4. **Matrix**: must be square (n × n)

### Example of Valid Input

```bash
echo "3 2 3 3 -4 2 5 0 -5 5" | ./build/debug/src/matrix_det
```

This represents the 3x3 matrix:
```
 2  3  3
-4  2  5
 0 -5  5
```

### Output Format

The program outputs the determinant of the matrix as a floating-point number.

### Example Output

```bash
190
```

## Level 2 — Matrix Chain Multiplication

Finds the optimal multiplication order for a chain of matrices using dynamic programming.
Outputs the lexicographically smallest optimal order and the speedup over naive left-to-right multiplication.

Complexity:
- DP cost table: $O(n^3)$
- Lexicographic tie-breaking over stored operation orders: worst-case up to $O(n^4)$

### Input Format

```
<dims_count> <dim_0> <dim_1> ... <dim_n>
```

1. **First number**: count of dimensions (number of matrices + 1)
2. **Next numbers**: dimensions, where matrix i has size dim_i x dim_(i+1)
3. **Minimum dims_count**: `2` (at least one matrix)
4. **Dimensions**: positive integers

### Example

```bash
echo "5 30 35 15 5 10" | ./build/debug/src/matrix_chain
1 0 2
2.08
```

This represents 4 matrices: 30x35, 35x15, 15x5, 5x10.
Output:
- line 1: optimal order `1 0 2`
- line 2: speedup factor `2.08` over naive order `0 1 2`

## Unit-tests

### How to Build

```bash
cmake -S . -B build/tests -G Ninja -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
cmake --build build/tests
```

### How to Run

```bash
ctest --test-dir build/tests --output-on-failure
```

## End-to-End Tests

Build debug binary first:

```bash
cmake -S . -B build/debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
cmake --build build/debug
```

Run determinant e2e tests:

```bash
./tests/end2end/det/run_e2e.sh ./build/debug/src/matrix_det
```

Run matrix-chain e2e tests:

```bash
./tests/end2end/chain/run_e2e.sh ./build/debug/src/matrix_chain
```
