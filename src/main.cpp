#include <iostream>

#include "better.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Incorrect number of parameters (" 
            << argc << "), expected 2\n";
        return 1;
    }

    std::string filename(argv[1]);
    Better::solve(filename);
    
    return 0;
}