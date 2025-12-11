import numpy as np

def write_test (test_num, matrix, det):
    with open (f'{test_num:03}.dat', 'w') as f:
        size = len (matrix)
        f.write (f'{size}')
        for row in matrix:
            for elem in row:
                f.write (f' {int(elem)}')
        f.write ('\n')

    with open (f'{test_num:03}.ans', 'w') as f:
        f.write (f'{int(det)}\n')

def generate_diagonal (test_num, size):
    if size >= 50:
        diag = [1] * size
        det = 1
    else:
        diag = [1 if i % 2 == 0 else 2 for i in range (size)]
        count_twos = size // 2
        det = 2 ** count_twos

    matrix = np.diag (diag)
    write_test (test_num, matrix, det)
    print (f' {test_num:03}: {size}x{size} diagonal, det={det}')
    return test_num + 1

def generate_identity (test_num, size):
    matrix = np.eye (size, dtype=int)
    det = 1

    write_test (test_num, matrix, det)
    print (f' {test_num:03}: {size}x{size} identity, det={det}')
    return test_num + 1

def generate_singular (test_num, size):
    matrix = np.random.randint (1, 10, (size, size))
    matrix[1] = matrix[0]
    det = 0

    write_test (test_num, matrix, det)
    print (f' {test_num:03}: {size}x{size} singular, det={det}')
    return test_num + 1

def generate_triangular (test_num, size):
    if size <= 5:
        matrix = np.triu (np.random.randint (1, 6, (size, size)))
        det = int (np.prod (np.diag (matrix)))
    elif size <= 10:
        matrix = np.triu (np.random.randint (0, 3, (size, size)))
        diag = np.random.randint (1, 4, size)
        np.fill_diagonal (matrix, diag)
        det = int (np.prod (diag))
    else:
        matrix = np.triu (np.random.randint (0, 3, (size, size)))
        np.fill_diagonal (matrix, 1)
        det = 1

    write_test (test_num, matrix, det)
    print (f' {test_num:03}: {size}x{size} upper triangular, det={det}')
    return test_num + 1

def generate_known (test_num, matrix_data, det, description):
    matrix = np.array (matrix_data)
    write_test (test_num, matrix, det)
    size = len (matrix)
    print (f' {test_num:03}: {size}x{size} ({description}), det={det}')
    return test_num + 1

if __name__ == '__main__':
    test_num = 1

    print ('=' * 60)
    print ('  Generating Matrix E2E Tests (Square Matrices Only)')
    print ('=' * 60)

    # small 2x2 matrices
    test_num = generate_known (test_num, [[1, 2], [3, 4]], -2, 'classic')
    test_num = generate_known (test_num, [[2, 0], [0, 3]], 6, 'diagonal')
    test_num = generate_known (test_num, [[1, 1], [1, 1]], 0, 'singular')

    # small 3x3 matrices
    test_num = generate_known (test_num, [[1, 2, 3], [0, 1, 4], [5, 6, 0]], 1, 'classic')
    test_num = generate_known (test_num, [[1, 0, 0], [0, 2, 0], [0, 0, 3]], 6, 'diagonal')
    test_num = generate_known (test_num, [[1, 2, 3], [2, 4, 6], [3, 5, 7]], 0, 'singular')

    # 4x4 and 5x5
    for size in [4, 5]:
        test_num = generate_diagonal (test_num, size)
        test_num = generate_identity (test_num, size)
        test_num = generate_triangular (test_num, size)

    # medium 10x10, 20x20
    for size in [10, 20]:
        test_num = generate_diagonal (test_num, size)
        test_num = generate_identity (test_num, size)
        test_num = generate_singular (test_num, size)
        test_num = generate_triangular (test_num, size)

    # large 30x30, 50x50
    for size in [30, 50]:
        test_num = generate_diagonal (test_num, size)
        test_num = generate_identity (test_num, size)
        test_num = generate_singular (test_num, size)
        test_num = generate_triangular (test_num, size)

    # very large 100x100, 200x200
    for size in [100, 200]:
        test_num = generate_identity (test_num, size)
        test_num = generate_diagonal (test_num, size)
        test_num = generate_singular (test_num, size)
        test_num = generate_triangular (test_num, size)

    print ('=' * 60)
    print (f'Generated {test_num - 1} test pairs!')
    print ('=' * 60)
