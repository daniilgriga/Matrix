![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)

# Matrix && Calculating its Determinant

## How to Install

```bash
git clone https://github.com/daniilgriga/Matrix.git
cd Matrix/
```

## How to Build

```bash
# Debug version:
cmake -S . -B build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DSANITIZE=ON
cmake --build build-debug

# Release version:
cmake -S . -B build-release -G Ninja -DCMAKE_BUILD_TYPE=Release -DSANITIZE=OFF -DBUILD_TESTS=OFF
cmake --build build-release
```

## How to Run

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
3 2 3 3 -4 2 5 0 -5 5
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

```
# in example 3 2 3 3 -4 2 5 0 -5 5:
190
```

## Unit-tests

### How to Build

```bash
cmake -S . -B build-tests -G Ninja -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
cmake --build build-tests
```

### How to Run

```bash
cd build-tests
ctest --output-on-failure
```
