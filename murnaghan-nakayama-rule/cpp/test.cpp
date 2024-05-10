#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestCppMurnaghanNakayama

#include <boost/test/included/unit_test.hpp>
#include "lib.h"

using PartitionT = Partition<size_t>;

BOOST_AUTO_TEST_SUITE(CharTableTest)

    struct CharTableCase {
        size_t n;
        std::vector<std::vector<BigInt>> char_table;
    };

    std::vector<CharTableCase> test_cases = {
            {3, {
                {1, -1, 1},
                {2, 0,  -1},
                {1, 1,  1},
            }},
            {4, {
                {1, -1, 1, 1, -1},
                {3, -1, -1, 0, 1},
                {2, 0,  2, -1, 0},
                {3, 1, -1, 0, -1},
                {1, 1, 1, 1,  1},
            }},
            {5, {
                {1, -1, 1, 1, -1, -1, 1},
                {4, -2, 0,  1, 1, 0, -1},
                {5, -1, 1, -1, -1, 1, 0},
                {6, 0, -2, 0, 0, 0, 1},
                {5, 1, 1, -1, 1, -1, 0},
                {4, 2, 0, 1, -1, 0, -1},
                {1, 1, 1, 1, 1, 1, 1},
            }},
    };

    void char_table_test(size_t n) {
        CharTableCase test_case = test_cases[n];
        auto calculator = CharTable();
        auto table = calculator.character_table(test_case.n);
        BOOST_CHECK(table == test_case.char_table);
    }

    BOOST_AUTO_TEST_CASE(case0) {char_table_test(0);}
    BOOST_AUTO_TEST_CASE(case1) {char_table_test(1);}
    BOOST_AUTO_TEST_CASE(case2) {char_table_test(2);}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PartitionsTest)

    struct PartitionsCase {
        size_t n;
        std::vector<PartitionT> partitions;
    };

    std::vector<PartitionsCase> test_cases = {
            {3, {
                PartitionT({1, 1, 1}),
                PartitionT({2, 1}),
                PartitionT({3}),
            }},
            {4, {
                PartitionT({1, 1, 1, 1}),
                PartitionT({2, 1, 1}),
                PartitionT({2, 2}),
                PartitionT({3, 1}),
                PartitionT({4}),
            }},
            {5, {
                PartitionT({1, 1, 1, 1, 1}),
                PartitionT({2, 1, 1, 1}),
                PartitionT({2, 2, 1}),
                PartitionT({3, 1, 1}),
                PartitionT({3, 2}),
                PartitionT({4, 1}),
                PartitionT({5}),
            }},
    };

    auto partitions_test(size_t n) {
        PartitionsCase test_case = test_cases[n];
        auto calculator = PartitionsCalculator();
        auto partitions = calculator.get(test_case.n);
        BOOST_CHECK(std::equal(partitions.begin(), partitions.end(),
                               test_case.partitions.begin()));
    }

    BOOST_AUTO_TEST_CASE(case0) {partitions_test(0);}
    BOOST_AUTO_TEST_CASE(case1) {partitions_test(1);}
    BOOST_AUTO_TEST_CASE(case2) {partitions_test(2);}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BorderStripsTest)

    struct BorderStripsCase {
        PartitionT lambda;
        size_t k;
        std::vector<PartitionWithoutBorderStrip> border_strips;
    };

    std::vector<BorderStripsCase> test_cases = {
            {PartitionT({5,2,1}),
             3, {
                {PartitionT({2, 2, 1}), {3}},
                {PartitionT({5}), {2, 1}},
            }},
            {PartitionT({4, 4, 3, 1}),
             5, {
                {PartitionT({3, 2, 1, 1}), {1, 2, 2}},
            }},
            {PartitionT({2, 1}),
             2,
             {}
            },
    };

    bool bs_equal (const PartitionWithoutBorderStrip& lhs, const PartitionWithoutBorderStrip& rhs) {
        return std::equal(lhs.partition.begin(), lhs.partition.end(), rhs.partition.begin()) and
            std::equal(lhs.border_strip.begin(), lhs.border_strip.end(), rhs.border_strip.begin());
    }

    auto border_strips_test(size_t n) {
        BorderStripsCase test_case = test_cases[n];
        auto calculator = CharTable();
        auto result = CharTable::get_border_strips(test_case.lambda, test_case.k);
        BOOST_CHECK(std::equal(result.begin(), result.end(),
                               test_case.border_strips.begin(), bs_equal));
    }

    BOOST_AUTO_TEST_CASE(case0) {border_strips_test(0);}
    BOOST_AUTO_TEST_CASE(case1) {border_strips_test(1);}
    BOOST_AUTO_TEST_CASE(case2) {border_strips_test(2);}

BOOST_AUTO_TEST_SUITE_END()