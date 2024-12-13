#!/bin/bash
cmake --build build/
build/main ./measurements.txt > /dev/null

time (build/main ./measurements.txt > result.out)
diff result.out solution.txt