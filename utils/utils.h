/**
 * @file process_image.h
 * @author Mauricio David Correa Hernández.
 * @brief:  Definition of the Image class - A utility class for handling image-related operations and displaying information.
 * @version 0.1
 * @date 2025-03-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <iomanip>

#include "../image/image.h" // Include of image library

using namespace std;

/**
 * @class UtilsImage
 * @brief A utility class for handling image-related operations and displaying information.
 */
class UtilsImage {
    public:
        /**
         * @brief Constructor for the UtilsImage class.
         */
        UtilsImage();

        /**
         * @brief Displays the correct usage of the program.
         * 
         * This function prints the correct command-line usage instructions.
         */
        void show_usage();

                /**
         * @brief Gets the size of an image file in memory.
         * 
         * This function calculates the approximate memory size required to store
         * an image by retrieving its dimensions and color channels.
         * 
         * @param file_name The path to the image file.
         * @return The size of the image in bytes.
         */
        size_t get_image_size(const std::string &file_name);

        /**
         * @brief Displays initial information about the image processing task.
         * 
         * This function prints details such as input and output file paths, 
         * rotation angle, scaling factor, and the memory allocation method used.
         * 
         * @param file_name Path of the input image.
         * @param exit_file_name Path of the output image.
         * @param use_buddy Flag indicating whether the Buddy System is used for memory allocation.
         * @param angle Rotation angle in degrees.
         * @param scalar Scaling factor for the image.
         */
        void initial_info(const std::string &file_name, const std::string &exit_file_name, bool use_buddy, int angle, float scalar);


        /**
         * @brief Displays the size of all images (original, scaled and rotated).
         * 
         * This function prints the details such as the width, high and channels of all images.
         * 
         * @param original_image_width Width of the original image.
         * @param original_image_high High of the original image.
         * @param scaled_image_width Width of the scaled image.
         * @param scaled_image_high High of the scaled image.
         * @param rotated_image_width Width of the rotated image.
         * @param rotated_image_high High of the rotated image.
         * @param channels Number of the channels of all images.
         */
        void show_images_size(int original_image_width, int original_image_high, int scaled_image_width, int scaled_image_high, int rotated_image_width, int rotated_image_high, int channels);

        /**
         * @brief Displays the total memory and time used in the execution.
         * 
         * This function prints the details such as the memory and time used in the execution.
         * 
         * @param time_usage Total time usage in the process.
         * @param memory_usage Total amount of memory usage in the process.
         * @param exit_file_name Exit file name.
         */
        void time_memory_usage(double time_usage, long memory_usage, const std::string &exit_file_name);

    private:
};

#endif
