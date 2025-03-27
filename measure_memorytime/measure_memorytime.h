/**
 * @file measure_memorytime.h
 * @author Mauricio David Correa Hernández.
 * @brief:  Definition of the Measure Memory and Time class - Class to measure memory usage and execution time for image processing tasks.
 * @version 0.1
 * @date 2025-03-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef MEASURE_MEMORY_AND_TIME_H
#define MEASURE_MEMORY_AND_TIME_H

#include <iostream>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <iomanip>

#include "../image/image.h" // Include of image library
#include "../process_image/process_image.h" // Include of process image library
#include "../utils/utils.h" // Include of process image library

using namespace std::chrono; // Brings everything from the std::chrono namespace into the current scope.

/**
 * @struct MemoryTimeStruct
 * @brief Structure to store the memory and time usage of an operation.
 */
struct MemoryTimeStruct{
    double time_used;   ///< Time taken for the operation in milliseconds.
    long memory_usage;  ///< Memory used for the operation in kilobytes.
};


class MeasureMemoryTime
{
    private:
        /**
         * @brief Calculates the nearest power of 2 greater than or equal to the given image size.
         * This is used for the Buddy memory allocator.
         * @param image_size The original image size.
         * @return The nearest power of 2 that is greater than or equal to `image_size`.
         */
        size_t get_buddy_size(size_t image_size);
    
    public:
        /**
         * @brief Constructor for MeasureMemoryTime.
         */
        MeasureMemoryTime();

        /**
         * @brief Measures memory usage and execution time using the Buddy memory allocator.
         * @param image_size Size of the image in bytes.
         * @param file_name Path to the input image file.
         * @param scalar Scaling factor for image resizing.
         * @param buddy Boolean flag indicating whether the Buddy allocator should be used.
         * @param angle Rotation angle in degrees.
         * @param exit_file_name Path to save the processed image.
         * @return A structure containing the memory usage and execution time.
         */
        MemoryTimeStruct MesureMemoryAndTimeForBuddyAllocator(size_t image_size, const std::string &file_name, float scalar, bool buddy, int angle, const std::string &exit_file_name);
        
        /**
         * @brief Measures memory usage and execution time using the `new/delete` memory allocation method.
         * @param file_name Path to the input image file.
         * @param scalar Scaling factor for image resizing.
         * @param buddy Boolean flag indicating whether the Buddy allocator should be used.
         * @param angle Rotation angle in degrees.
         * @param exit_file_name Path to save the processed image.
         * @return A structure containing the memory usage and execution time.
         */
        MemoryTimeStruct MesureMemoryAndTimeForNewDelete(const std::string &file_name,float scalar, bool buddy, int angle, const std::string &exit_file_name);
        
        /**
         * @brief Retrieves the memory usage of the current process.
         * @return Memory usage in kilobytes.
         */
        long get_memory_usage();
};

#endif