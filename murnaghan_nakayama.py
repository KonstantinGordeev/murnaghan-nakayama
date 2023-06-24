import typing as tp
from collections import defaultdict


class CharTable(object):
    """
    Class for calculation the values of the character table of the finite symmetric groups
    The values are calculated using the Murnaghan–Nakayama rule
    """

    def __init__(self) -> None:
        
        # TODO: list for n (degree of the group)
        self.character_values: tp.Dict[tp.Tuple[int, ...], tp.Dict[tp.Tuple[int, ...], int]] = defaultdict(dict)

        # for every n item contains list of partitions. Initialize for n = 0 and n = 1
        self.partitions: tp.List[tp.List[tp.Tuple[int, ...]]] = [[()], [(1,)]]

    @staticmethod
    def get_border_strips(lambda_: tp.Tuple[int, ...], k: int) -> tp.Generator[None, tp.Tuple[tp.List[int],
                                                                               tp.List[int]], None]:
        """
        Return generator of sub-partition lambda* of lambda_ and border strip xi_ of sum k such that
        lambda* + xi_ = lambda_, lambda* is a correct partition and xi_ is connected and has no 2x2 parts
        Computational complexity is O(lambda_[0] + len(lambda_))
        """
        
        row = 0
        xi_ = [0] * len(lambda_)
        lambda_ = list(lambda_)
        first_non_zero_row = 0

        while k > 0 and row < len(lambda_):

            if row + 1 == len(lambda_):  # last left step
                step = min(k, lambda_[row])
                k -= step
                lambda_[row] -= step
                xi_[row] += step

                if k > 0:
                    return

            elif lambda_[row] > lambda_[row + 1]:  # left step
                step = min(k, lambda_[row] - lambda_[row + 1])
                k -= step
                lambda_[row] -= step
                xi_[row] += step

            else:  # down step
                prev_row = row
                while row < len(lambda_) and lambda_[row] == lambda_[prev_row]:
                    k -= 1
                    xi_[row] += 1
                    row += 1

                for index in range(prev_row, row):
                    lambda_[index] -= 1

                row -= 1

                while k < 0:
                    lambda_[first_non_zero_row] += xi_[first_non_zero_row]
                    k += xi_[first_non_zero_row]
                    xi_[first_non_zero_row] = 0
                    first_non_zero_row += 1

            if k == 0:
                yield lambda_, xi_
                    
                lambda_[first_non_zero_row] += xi_[first_non_zero_row]
                k += xi_[first_non_zero_row]
                xi_[first_non_zero_row] = 0
                if row == first_non_zero_row:
                    row += 1
                first_non_zero_row += 1

        return

    def calculate_partitions(self, n: int) -> tp.List[tp.Tuple[int, ...]]:
        """
        Dynamically calculates partitions for every k in interval [len(self.partitions) + 1, n]
        Values caches in 'self.partitions'
        Returns list of partitions for n
        """

        for num in range(len(self.partitions), n + 1):
            partitions_of_num = set()
            for i in range(num):
                for partition in self.partitions[i]:
                    partitions_of_num.add(tuple(sorted((num - i,) + partition, reverse=True)))
            self.partitions.append(sorted(list(partitions_of_num)))
        return self.partitions[n]

    @staticmethod
    def correct_partition(lambda_: tp.Tuple[int, ...]) -> tp.Tuple[int, ...]:
        """
        Remove zeros from partition
        """
        return tuple(item for item in lambda_ if item != 0)

    def char_value(self, lambda_: tp.Tuple[int, ...], rho_: tp.Tuple[int, ...]) -> int:
        """
        Calculates character value of irreducible character on conjugacy class
        lambda_: irreducible character
        rho_: conjugacy class
        return: value irreducible character of conjugacy class and save it into table
        """

        lambda_ = self.correct_partition(lambda_)
        rho_ = self.correct_partition(rho_)

        if sum(lambda_) < 2:
            return 1

        value = self.character_values[lambda_].get(rho_)
        if value is not None:
            return value

        result = 0
        for partition, xi_ in self.get_border_strips(lambda_, rho_[0]):
            partition = self.correct_partition(partition)
            xi_ = self.correct_partition(xi_)
            result += (-1) ** (len(xi_) - 1) * self.char_value(partition, rho_[1:])

        self.character_values[lambda_][rho_] = result

        return result

    def character_table(self, n: int) -> tp.List[tp.List[int]]:
        """
        Calls char_value(...) for every irreducible character and conjugacy class
            of symmetric group of degree n
        return: square table of character values
        """
        partitions: tp.List[tp.Tuple[int, ...]] = self.calculate_partitions(n)
        table: tp.List[tp.List[int]] = []

        for lambda_ in partitions:
            new_line = []
            for rho_ in partitions:
                new_line.append(self.char_value(lambda_, rho_))
            table.append(new_line.copy())

        return table
