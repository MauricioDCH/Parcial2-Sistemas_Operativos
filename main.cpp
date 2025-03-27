/**
 * @file main.cpp
 * @author Mauricio David Correa Hernández.
 * @brief:  Implementation of the main program to run the project.
 * @version 0.1
 * @date 2025-03-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include <cstring>

#include "utils/utils.h" // Include of utils library
#include "measure_memorytime/measure_memorytime.h" // Include of measure_memorytime library

using namespace std;

int main(int argc, char* argv[]) {
    UtilsImage utils;
    MeasureMemoryTime measureMemoryTime;
    MemoryTimeStruct measureMemoryTimeStruct;
    
    // Check number of arguments
    if (argc != 7 && argc != 8) {
        cerr << "Error: Number of arguments is incorrect." << endl;
        utils.show_usage();
        return 1;
    }

    // Command line parameters
    string file_name = argv[1];
    string exit_file_name = argv[2];
    int angle = std::stoi(argv[4]);
    float scalar = std::stof(argv[6]);
    string assignment_mode = (argc == 8) ? argv[7] : "";

    // Check if the assignment mode is valid
    bool buddy = false;
    if (assignment_mode == "-buddy") {
        buddy = true;
    } else if (assignment_mode == "") {
        buddy = false;
    } else {
        cerr << "Error: Invalid mode option." << endl;
        utils.show_usage();
        return 1;
    }

    // Show initial information
    utils.initial_info(file_name, exit_file_name, buddy, angle, scalar);

    // Project execution
    // If buddy is true, then run the project with Buddy System.
    // Otherwise, run the project with new/delete.
    if (buddy) {
        size_t image_size = utils.get_image_size(file_name);
        measureMemoryTimeStruct = measureMemoryTime.MesureMemoryAndTimeForBuddyAllocator(image_size, file_name, scalar, buddy, angle, exit_file_name);
    } else {
        measureMemoryTimeStruct = measureMemoryTime.MesureMemoryAndTimeForNewDelete(file_name, scalar, buddy, angle, exit_file_name);
    }

    // Show time and memory usage.
    utils.time_memory_usage(measureMemoryTimeStruct.time_used, measureMemoryTimeStruct.memory_usage, exit_file_name);

    return 0;
}
