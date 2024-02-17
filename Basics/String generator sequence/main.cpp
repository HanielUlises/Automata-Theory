#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ranges>

// Utilize std::string_view for static data to avoid unnecessary copies
constexpr std::string_view alphabet[] = {"a", "b"};

// Function to generate and write the specific series of strings for the alphabet {a, b} directly to a file
void generateAndWriteSeries(int seriesLength, const std::string& fileName) {
    std::ofstream outFile(fileName);
    if (!outFile.is_open()) {
        std::cout << "Failed to open file for writing." << std::endl;
        return;
    }

    // Write the initial empty set
    outFile << "∅";
    
    // Generate and write the series up to the specified length, capped at 1000
    for (int i = 0; i < seriesLength && i < std::size(alphabet); ++i) {
        outFile << ", " << alphabet[i];
    }

    // Special case for concatenating 'a' and 'b' to form 'ab'
    if (seriesLength > 2) {
        outFile << ", ab";
    }

    std::cout << "Series written to " << fileName << std::endl;
}

int main() {
    int seriesLength;
    std::cout << "Enter the series length (up to 1000): ";
    std::cin >> seriesLength;

    seriesLength = std::min(seriesLength, 1000);

    generateAndWriteSeries(seriesLength, "string_sequence.txt");

    return 0;
}
