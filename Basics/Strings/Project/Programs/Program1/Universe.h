#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include <limits>
#include <numeric>

#include "Plot.h"

// Utilized std::string_view for static data to avoid unnecessary copies
// BECAUSE SETS DO NOT HAVE REPETITIONS
constexpr std::string_view alphabet[] = {"0", "1"};

// Recursive function to generate all combinations of the alphabet
void generateCombinations(const std::string& prefix, int length, std::ofstream& outFile, std::vector<int>& ones) {
    if (length == 0) {
        if (!prefix.empty()) {
            outFile << ", " << prefix;
            int onesCount = std::count(prefix.begin(), prefix.end(), '1');
            ones.push_back(onesCount);
        }
        return;
    }
    
    for (const auto& letter : alphabet) {
        generateCombinations(prefix + std::string(letter), length - 1, outFile, ones);
    }
}

void generateAndWriteSeries(int seriesLength, const std::string& fileName, std::vector<int>& ones) {
    std::ofstream outFile(fileName);
    if (!outFile.is_open()) {
        std::cout << "Failed to open file for writing." << std::endl;
        return;
    }

    // Write the initial empty set
    // This is {a,b} ^ 0
    outFile << "{ε";

    for (int i = 1; i <= seriesLength; ++i) {
        generateCombinations("", i, outFile, ones);
    }

    outFile << "}";
    
    std::cout << "Series succesfully written to " << fileName << std::endl;
}

void main_func() {
    int n;
    std::cout << "Choose a number or let the computer choose one: ";
    std::vector<int> ones;
    
    std::cin >> n;
    
    if (n == 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, 10);

        n = distrib(gen);
        std::cout << "Randomly chosen number: " << n << std::endl;
    }

    if (n > 28) {
        std::cout << "Too big." << std::endl;
        n = 28;
    }

    generateAndWriteSeries(n, "Programs/output.txt", ones);
    std::cout << "Contents of vector 'ones': ";
    for (const int& count : ones) {
        std::cout << count << " ";
    }
    std::cout << std::endl;
    plotHistogram(ones);
}
