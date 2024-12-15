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

namespace {

inline void print_fixed(bool is_negative, int64_t non_negative) {
    if (is_negative) std::cout << '-';
    std::cout << non_negative / 10 << '.' << non_negative % 10; 
} 

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

        std::cout << name << '=';
        print_fixed(min < 0, std::abs(min));
        std::cout << '/';
        print_fixed(is_negative, sum);
        std::cout << '/';
        print_fixed(max < 0, std::abs(max));
    }
};

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

        // Also, it's faster than rawmemchr for some reason
        // curr = static_cast<const char*>(memchr(curr, ';', final - curr)); // slowest
        // curr = static_cast<const char *>(rawmemchr(curr, ';'));
        curr = strchr(curr, ';');

        size_t idx = static_cast<size_t>(curr - start_of_row);

        // string_view is okay here, because we only unmmap AFTER we are done
        // outputing the result
        std::string_view station_name(start_of_row, idx);

        // Parse the measurement manually into a fixed point integer
        int64_t sign = 1;
        if (*++curr == '-') {
            curr++;
            sign = -1;
        }

        int64_t measurement = (*curr - '0') * 10;
        ++curr;

        if (*curr != '.') {
            measurement = (measurement + *curr - '0') * 10;
            ++curr;
        }

        ++curr;
        measurement += *curr - '0';
        measurement *= sign;

        Information& info = measurements[station_name];
        info.num_measurements++;
        info.sum += measurement;
        info.max = std::max(info.max, measurement);
        info.min = std::min(info.min, measurement);

        curr += 2;
    }

    std::vector<std::pair<std::string_view, Information>> all(measurements.begin(), measurements.end());
    std::sort(all.begin(), all.end(), [](auto &lhs, auto &rhs){
        return lhs.first < rhs.first;
    });

    std::cout << '{';

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