import time
from murnaghan_nakayama import CharTable

if __name__ == "__main__":
    test = CharTable()

    time_for_n = []

    for n in range(1, 30):
        rho_1 = tuple([4] * n)
        rho_2 = tuple([4] * (n - 1) + [3, 1])
        rho_3 = tuple([2] * (2 * n - 1) + [1] * 2)
        rho_4 = tuple([1] * (4 * n))

        start = time.time()

        for lambda_ in test.calculate_partitions(4 * n):
            test.char_value(lambda_, rho_1)
            test.char_value(lambda_, rho_2)
            test.char_value(lambda_, rho_3)
            test.char_value(lambda_, rho_4)

        end = time.time()
        print(n, end - start)