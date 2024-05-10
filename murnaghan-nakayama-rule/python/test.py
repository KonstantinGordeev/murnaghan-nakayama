import dataclasses
import pytest

from lib import CharTable


@dataclasses.dataclass
class CharTableCase:
    n: int
    char_table: list[list[int]]


CharTableTests = [
    CharTableCase(
        n=3,
        char_table=[
            [1, -1, 1],
            [2, 0, -1],
            [1, 1, 1],
        ]
    ),
    CharTableCase(
        n=4,
        char_table=[
            [1, -1, 1, 1, -1],
            [3, -1, -1, 0, 1],
            [2, 0, 2, -1, 0],
            [3, 1, -1, 0, -1],
            [1, 1, 1, 1, 1],
        ]
    ),
    CharTableCase(
        n=5,
        char_table=[
            [1, -1, 1, 1, -1, -1, 1],
            [4, -2, 0, 1, 1, 0, -1],
            [5, -1, 1, -1, -1, 1, 0],
            [6, 0, -2, 0, 0, 0, 1],
            [5, 1, 1, -1, 1, -1, 0],
            [4, 2, 0, 1, -1, 0, -1],
            [1, 1, 1, 1, 1, 1, 1],
        ]
    ),
]


@dataclasses.dataclass
class PartitionsCase:
    n: int
    partitions: list[tuple[int, ...]]


PartitionsTests = [
    PartitionsCase(
        n=3,
        partitions=[
            (1, 1, 1),
            (2, 1),
            (3,)
        ]
    ),
    PartitionsCase(
        n=4,
        partitions=[
            (1, 1, 1, 1),
            (2, 1, 1),
            (2, 2),
            (3, 1),
            (4,)
        ]
    ),
    PartitionsCase(
        n=5,
        partitions=[
            (1, 1, 1, 1, 1),
            (2, 1, 1, 1),
            (2, 2, 1),
            (3, 1, 1),
            (3, 2),
            (4, 1),
            (5,),
        ]
    ),
]


@dataclasses.dataclass
class BorderStripsCase:
    lambda_: tuple[int, ...]
    k: int
    border_strips: list[tuple[tuple[int, ...], tuple[int, ...]]]


BorderStripsTests = [
    BorderStripsCase(
        lambda_=(5, 2, 1),
        k=3,
        border_strips=[
            ((2, 2, 1), (3, 0, 0)),
            ((5, 0, 0), (0, 2, 1)),
        ]
    ),
    BorderStripsCase(
        lambda_=(4, 4, 3, 1),
        k=5,
        border_strips=[
            ((3, 2, 1, 1), (1, 2, 2, 0)),
        ]
    ),
    BorderStripsCase(
        lambda_=(2, 1),
        k=2,
        border_strips=[]
    ),
]


@pytest.mark.parametrize('test_case', CharTableTests)
def test_char_table(test_case: CharTableCase) -> None:
    calculator = CharTable()
    table = calculator.character_table(test_case.n)
    assert table == test_case.char_table


@pytest.mark.parametrize('test_case', PartitionsTests)
def test_partitions(test_case: PartitionsCase) -> None:
    calculator = CharTable()
    partitions = calculator.calculate_partitions(test_case.n)
    assert partitions == test_case.partitions


@pytest.mark.parametrize('test_case', BorderStripsTests)
def test_get_border_strips(test_case: BorderStripsCase) -> None:
    generator = CharTable.get_border_strips(test_case.lambda_, test_case.k)
    result = [(tuple(item[0]), tuple(item[1])) for item in generator if item is not None]
    assert result == test_case.border_strips
