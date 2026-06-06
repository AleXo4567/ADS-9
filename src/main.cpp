// Copyright 2022 NNTU-CS
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <random>
#include <vector>
#include "tree.h"

int main() {
    std::vector<char> start = { '1', '2', '3' };
    PMTree testTree(start);

    auto perms = getAllPerms(testTree);
    for (auto& p : perms) {
        for (char c : p) std::cout << c;
        std::cout << "  ";
    }
    std::cout << "\n\n";

    std::cout << "getPerm1(1): ";
    auto firstPerm = getPerm1(testTree, 1);
    for (char c : firstPerm) std::cout << c;
    std::cout << std::endl;

    std::cout << "getPerm2(2): ";
    auto secondPerm = getPerm2(testTree, 2);
    for (char c : secondPerm) std::cout << c;
    std::cout << "\n\n";

    std::cout << "n;getAllPerms(s);getPerm1(s);getPerm2(s)" << std::endl;

    for (int n = 3; n <= 8; ++n) {
        std::vector<char> symbols;
        for (int i = 0; i < n; ++i) symbols.push_back('a' + i);
        PMTree curTree(symbols);

        std::mt19937 rng(42);
        std::uniform_int_distribution<int> range(
            1, static_cast<int>(fact(n)));
        int targetPos = range(rng);

        auto startTime = std::chrono::high_resolution_clock::now();
        getAllPerms(curTree);
        auto afterAll = std::chrono::high_resolution_clock::now();
        getPerm1(curTree, targetPos);
        auto afterFirst = std::chrono::high_resolution_clock::now();
        getPerm2(curTree, targetPos);
        auto endTime = std::chrono::high_resolution_clock::now();

        double elapsedAll = std::chrono::duration<double>(
            afterAll - startTime).count();
        double elapsedFirst = std::chrono::duration<double>(
            afterFirst - afterAll).count();
        double elapsedSecond = std::chrono::duration<double>(
            endTime - afterFirst).count();

        std::cout << n << ";"
                  << std::fixed << std::setprecision(6) << elapsedAll << ";"
                  << elapsedFirst << ";"
                  << elapsedSecond << std::endl;
    }

    return 0;
}
