#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <limits>
#include <iostream>
#include <sys/types.h>
#include <vector>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>    
#include <unistd.h>   
#include <cstring>    

#include "solutions.h"

#include "../../unordered_dense_install/include/ankerl/unordered_dense.h"

struct Information {
    // Fixed point, one decimal places
    int64_t min = std::numeric_limits<int64_t>().max();
    int64_t max = std::numeric_limits<int64_t>().min();
    int64_t sum = 0;
    int64_t num_measurements = 0;

    inline void print(std::string_view name) const {
        bool is_negative = this->sum < 0;
        int64_t sum = std::abs(this->sum) * 10;

        sum /= num_measurements;
        sum += 5;
        sum /= 10;

        std::cout 
            << name << '=' 
            << static_cast<double>(min) / 10 << '/'
            << (is_negative ? "-" : "") << sum / 10 << '.' << sum % 10 << '/'
            << static_cast<double>(max) / 10;
    }
};

inline int64_t parse_measurement(const char *s, size_t size) {
    bool is_negative = s[0] == '-';
    int64_t result = 0;
    for (size_t i = is_negative; i < size - 2; i++) {
        result = 10 * result + s[i] - '0';
    }

    result = 10 * result + s[size - 1] - '0';

    if (is_negative) result *= -1;
    return result;
}

void Solutions::memory_map(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Unable to open provided file: " << filename << '\n';
        return;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        std::cerr << "Unable to seek to end of file.";
        close(fd);
        return;
    }
    size_t fileSize = static_cast<size_t>(st.st_size);

    void *mapped = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        std::cerr << "Unable to mmap file.";
        close(fd);
        return;
    }
    
    const char *curr = static_cast<const char *>(mapped);

    ankerl::unordered_dense::map<std::string_view, Information> measurements;
    measurements.reserve(5000);

    const char *final = curr + fileSize;
    while (curr < final) {
        // Stores the start of the station name/measurement
        const char *start_of_row = curr; 

        // NOTE: mmap isn't null terminated, but the input guarantees that there
        // will exist ; somewhere in the remainder of the string which we have
        // yet to read.
        curr = strchr(curr, ';');
        size_t idx = static_cast<size_t>(curr - start_of_row);

        // string_view is okay here, because we only unmmap AFTER we are done
        // outputing the result
        std::string_view station_name(start_of_row, idx);

        const char *end_of_row = strchr(curr, '\n');
        size_t n = static_cast<size_t>(end_of_row - start_of_row);

        int64_t measurement = parse_measurement(curr + 1, n - idx - 1);

        Information& info = measurements[station_name];
        info.num_measurements++;
        info.sum += measurement;
        info.max = std::max(info.max, measurement);
        info.min = std::min(info.min, measurement);

        curr = end_of_row + 1;
    }

    std::vector<std::pair<std::string_view, Information>> all(measurements.begin(), measurements.end());
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

    // Clean up
    munmap(mapped, fileSize);
    close(fd);
}