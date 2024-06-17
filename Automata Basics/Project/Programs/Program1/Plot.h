#include "matplotlibcpp.h"
#include <vector>
#include <algorithm>

namespace plt = matplotlibcpp;

void plotHistogram(const std::vector<int>& display_vec) {
    if (display_vec.empty()) {
        std::cout << "The vector is empty. No histogram to display." << std::endl;
        return;
    }

    auto [min, max] = std::minmax_element(display_vec.begin(), display_vec.end());

    long numberOfBins = *max - *min + 1;

    plt::hist(display_vec, numberOfBins, "b"); 

    std::vector<double> ticks;
    for (int i = *min; i <= *max; ++i) {
        ticks.push_back(i);
    }
    plt::xticks(ticks);

    plt::title("Histogram of Ones");
    plt::xlabel("Number of Ones");
    plt::ylabel("Frequency");

    plt::show();
}