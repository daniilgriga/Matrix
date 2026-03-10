import random

def mcm_solve(dims):
    n = len(dims) - 1
    cost = [[0] * n for _ in range(n)]
    split = [[0] * n for _ in range(n)]
    order = [[[] for _ in range(n)] for _ in range(n)]

    for length in range(2, n + 1):
        for i in range(n - length + 1):
            j = i + length - 1
            cost[i][j] = float('inf')
            for k in range(i, j):
                c = cost[i][k] + cost[k+1][j] + dims[i] * dims[k+1] * dims[j+1]
                candidate = order[i][k] + order[k+1][j] + [k]
                if c < cost[i][j] or (c == cost[i][j] and candidate < order[i][j]):
                    cost[i][j] = c
                    split[i][j] = k
                    order[i][j] = candidate

    return cost[0][n-1], order[0][n-1]

def naive_cost(dims):
    n = len(dims) - 1
    if n < 2:
        return 0
    cost = 0
    rows = dims[0]
    for i in range(1, n):
        cost += rows * dims[i] * dims[i+1]
        rows = dims[0]
    return cost

def write_test(test_num, dims, description):
    n = len(dims) - 1
    optimal, order = mcm_solve(dims)

    naive = naive_cost(dims)
    speedup = naive / optimal if optimal > 0 else 1.0

    with open(f'{test_num:03}.dat', 'w') as f:
        f.write(f'{len(dims)}')
        for d in dims:
            f.write(f' {d}')
        f.write('\n')

    with open(f'{test_num:03}.ans', 'w') as f:
        f.write(' '.join(str(k) for k in order) + '\n')
        f.write(f'{speedup:.6g}\n')

    print(f' {test_num:03}: {n} matrices ({description}), speedup={speedup:.4f}')
    return test_num + 1


if __name__ == '__main__':
    random.seed(42)
    # tests 001-010 are hand-written, start from 011
    test_num = 11

    print('=' * 60)
    print('  Generating MatrixChain E2E Tests (auto, 11+)')
    print('=' * 60)

    dims = [random.randint(5, 100) for _ in range(12)]
    test_num = write_test(test_num, dims, '11 random')

    dims = [random.randint(5, 100) for _ in range(13)]
    test_num = write_test(test_num, dims, '12 random')

    dims = [200, 180, 150, 120, 100, 80, 60, 50, 40, 30, 20, 15, 10, 5]
    test_num = write_test(test_num, dims, '13 decreasing')

    dims = [5, 10, 15, 20, 30, 40, 50, 60, 80, 100, 120, 150, 180, 200, 250]
    test_num = write_test(test_num, dims, '14 increasing')

    dims = [random.randint(5, 200) for _ in range(16)]
    test_num = write_test(test_num, dims, '15 random')

    dims = [random.randint(2, 20) for _ in range(17)]
    test_num = write_test(test_num, dims, '16 small dims')

    dims = [random.randint(1, 300) for _ in range(19)]
    test_num = write_test(test_num, dims, '18 wide range')

    dims = [random.randint(5, 150) for _ in range(21)]
    test_num = write_test(test_num, dims, '20 matrices')

    dims = []
    for i in range(21):
        if i % 3 == 0:
            dims.append(random.randint(1, 5))
        else:
            dims.append(random.randint(50, 300))
    test_num = write_test(test_num, dims, '20 extreme bottleneck')

    dims = [random.randint(5, 100) for _ in range(26)]
    test_num = write_test(test_num, dims, '25 matrices')

    dims = [random.randint(3, 80) for _ in range(31)]
    test_num = write_test(test_num, dims, '30 matrices')

    dims = [random.randint(2, 50) for _ in range(51)]
    test_num = write_test(test_num, dims, '50 matrices')

    print('=' * 60)
    print(f'Generated {test_num - 11} auto test pairs (011-{test_num-1:03})!')
    print('=' * 60)
