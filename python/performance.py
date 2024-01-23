import time
import typing as tp
from lib import CharTable


def calculate_all_irreducible_characters_for_4_conjugacy_classes(sequentially: int = True, max_n: int = 30):

    test = CharTable()

    cumulative_time = 0
    
    for n in range(1, max_n):
        if not sequentially:
            test.__init__()

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
        cumulative_time += end - start
        print(n, end - start, cumulative_time, sep="\t")


def calculate_char_tables(sequentially: int = True, max_n: int = 31):
    test = CharTable()

    cumulative_time = 0

    for n in range(1, max_n):
        if not sequentially:
            test.__init__()

        start = time.time()

        test.character_table(n)

        end = time.time()
        cumulative_time += end - start
        print(n, end - start, cumulative_time, sep="\t")


if __name__ == "__main__":
    calculate_char_tables(False)
