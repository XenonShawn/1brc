#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <ios>
#include <limits>
#include <iostream>
#include <vector>

#include "better.h"

#include "../../unordered_dense_install/include/ankerl/unordered_dense.h"

struct Information {
    // Fixed point, two decimal places
    int64_t min = std::numeric_limits<int64_t>().max();
    int64_t max = std::numeric_limits<int64_t>().min();
    int64_t sum = 0;
    int64_t num_measurements = 0;

    inline void print(std::string_view name) const {
        bool is_negative = this->sum < 0;
        int64_t sum = std::abs(this->sum);

        sum /= num_measurements;
        sum += 5;
        sum /= 10;

        std::cout 
            << name << '=' 
            << static_cast<double>(min) / 100 << '/'
            << (is_negative ? "-" : "") << sum / 10 << '.' << sum % 10 << '/'
            << static_cast<double>(max) / 100;
    }
};

inline int64_t parse_measurement(const char *s, size_t size) {
    bool is_negative = s[0] == '-';
    int64_t result = 0;
    for (size_t i = is_negative; i < size; i++) {
        result = 10 * result + s[i] - '0';
    }

    result *= 10;

    if (is_negative) result *= -1;
    return result;
}

void Better::solve(std::string filename) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cerr << "Unable to open provided file: " << filename << '\n';
        return;
    }

    ankerl::unordered_dense::map<std::string, Information> measurements;
    measurements.reserve(5000);
    {
        std::string row;
        while (std::getline(input, row)) {
            size_t idx = row.find_last_of(';');

            size_t n = row.size();
            row[n - 2] = row[n - 1];
            int64_t measurement = parse_measurement(row.c_str() + idx + 1, n - idx - 2);
            row.erase(idx);

            Information& info = measurements[row];
            info.num_measurements++;
            info.sum += measurement;
            info.max = std::max(info.max, measurement);
            info.min = std::min(info.min, measurement);
        }
    }

    std::vector<std::pair<std::string, Information>> all(measurements.begin(), measurements.end());
    std::sort(all.begin(), all.end(), [](auto &lhs, auto &rhs){
        return lhs.first < rhs.first;
    });

    std::cout << std::fixed << std::setprecision(1) << '{';

    auto it = all.begin();
    it->second.print(it->first);

    for (it++; it != all.end(); it++) {
        std::cout << ", ";
        it->second.print(it->first);
    }
    std::cout << "}\n";
}