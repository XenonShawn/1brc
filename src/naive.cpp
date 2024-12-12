#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <string_view>
#include <utility>

struct Information {
    // Fixed point, with two decimal places
    double min = std::numeric_limits<double>().max();
    double max = std::numeric_limits<double>().min();
    double sum = 0;
    int64_t num_measurements = 0;

    inline void print(std::string_view name) const {
        std::cout 
            << name << '=' 
            << min << '/'
            << round(sum / num_measurements * 10.0) / 10.0 << '/'
            << max;
    }
};

/**
 * This solution for the 1brc does the simplest reasonable method:
 * - Read input through a file stream
 * - Process each row sequentially, keeping track of all information
 * - Uses a map to keep track of every weather station.
 */
int naive(std::string filename) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cerr << "Unable to open provided file: " << filename << '\n';
        return 1;
    }

    std::map<std::string, Information> measurements;
    {
        std::string row;
        while (std::getline(input, row)) {
            // Each for is one measurement in the format 
            // <string: station name>;<double: measurement>
            size_t idx = row.find(';');
            std::string station_name = row.substr(0, idx);
            std::string measurement_str = row.substr(idx + 1);
            double measurement = std::stod(measurement_str);

            Information& info = measurements[station_name];
            info.num_measurements++;
            info.sum += measurement;
            info.max = std::max(info.max, measurement);
            info.min = std::min(info.min, measurement);
        }
    }

    std::cout << std::fixed << std::setprecision(1) << '{';

    auto it = measurements.begin();
    it->second.print(it->first);

    for (it++; it != measurements.end(); it++) {
        std::cout << ", ";
        it->second.print(it->first);
    }
    std::cout << "}\n";

    return 0;
}
