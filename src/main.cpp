#include <iostream>

#include "solutions.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Incorrect number of parameters (" 
            << argc << "), expected 2\n";
        return 1;
    }

    Solutions::multithreaded(argv[1]);
    
    return 0;
}