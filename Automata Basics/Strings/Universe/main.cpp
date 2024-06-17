#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include <limits>

// Utilized std::string_view for static data to avoid unnecessary copies
// BECAUSE SETS DO NOT HAVE REPETITIONS
constexpr std::string_view alphabet[] = {"a", "b"};

// Recursive function to generate all combinations of the alphabet
void generateCombinations(const std::string& prefix, int length, std::ofstream& outFile) {
    if (length == 0) {
        if (!prefix.empty()) { // Skip the empty prefix case, as it's handled separately
            outFile << ", " << prefix;
        }
        return;
    }
    
    for (const auto& letter : alphabet) {
        generateCombinations(prefix + std::string(letter), length - 1, outFile);
    }

}

// Function to generate and write the specific series of strings for the alphabet {a, b} directly to a file
void generateAndWriteSeries(int seriesLength, const std::string& fileName) {
    std::ofstream outFile(fileName);
    if (!outFile.is_open()) {
        std::cout << "Failed to open file for writing." << std::endl;
        return;
    }

    // Write the initial empty set
    // This is {a,b}  0
    outFile << "{ε";

    // Generate and write the series up to the specified length, capped at 1000
    for (int i = 1; i <= seriesLength; ++i) {
        generateCombinations("", i, outFile);
    }

    outFile << "}";
    
    std::cout << "Series succesfully written to " << fileName << std::endl;
}

int main(int argc, char* argv[]) {
    int seriesLength = 0;
    std::cout << "Enter the power (or press Enter to let the computer choose): ";

    std::cin >> seriesLength;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 1000);

        seriesLength = dist(gen);
        std::cout << "Computer-selected power: " << seriesLength << std::endl;
    } else {
            seriesLength = std::min(seriesLength, 1000);
    }

    generateAndWriteSeries(seriesLength, "string_sequence.txt");

    return 0;
}
