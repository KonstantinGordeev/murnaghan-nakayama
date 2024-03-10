#include <iostream>
#include <ctime>
#include "lib.h"

void calculate_all_irreducible_characters_for_4_conjugacy_classes(bool sequentially = true, size_t max_n = 30) {
    auto test = CharTable();

    double cumulative_time = 0;

    for (size_t n = 1; n < max_n; ++n) {
        if (!sequentially) {
            test = CharTable();
        }

        auto rho_1 = Partition(n, 4);
        auto rho_2 = Partition(n - 1, 4);
        rho_2.emplace_back(3);
        rho_2.emplace_back(1);
        auto rho_3 = Partition(2 * n - 1, 2);
        rho_3.emplace_back(1);
        rho_3.emplace_back(1);
        auto rho_4 = Partition(4 * n, 1);

        clock_t start = clock();

        for (const auto& lambda: *test.calculate_partitions(4 * n)) {
            test.char_value(lambda, rho_1);
            test.char_value(lambda, rho_2);
            test.char_value(lambda, rho_3);
            test.char_value(lambda, rho_4);
        }

        clock_t end = clock();
        double time_diff = static_cast<double>(end - start) / CLOCKS_PER_SEC;
        cumulative_time += time_diff;

        std::cout << n << "\t" << time_diff << "\t" << cumulative_time << std::endl;
    }
}

void calculate_char_tables(bool sequentially = true, size_t max_n = 31) {
    auto test = CharTable();

    double cumulative_time = 0;

    for (size_t n = 1; n < max_n; ++n) {
        if (!sequentially) {
            test = CharTable();
        }

        clock_t start = clock();

        test.character_table(n);

        clock_t end = clock();
        double time_diff = static_cast<double>(end - start) / CLOCKS_PER_SEC;
        cumulative_time += time_diff;

        std::cout << n << "\t" << time_diff << "\t" << cumulative_time << std::endl;
    }
}