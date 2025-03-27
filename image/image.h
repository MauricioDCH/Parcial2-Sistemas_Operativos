/**
 * @file image.h
 * @author Mauricio David Correa Hernández.
 * @brief:  Definition of the Image class - Class representing an image and allowing its manipulation.
 * @version 0.1
 * @date 2025-03-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <iostream>


#include "../stb/stb_image.h" // Include of stb_image library
#include "../stb/stb_image_write.h" // Include of stb_image_write library
#include "../buddy_system/buddy_allocator.h" // Include of buddy_allocator library


using namespace std;


/**
 * @brief Structure that stores information about a processed image.
 */
struct Image_info{
    unsigned char* new_data;        ///< Data of the new image.
    int new_width;                  ///< Width of the new image.
    int new_high;                   ///< High of the new image.
    long memory_used_for_process;   ///< Memory used for the process in execution.
};


/**
 * @brief Class representing an image and allowing its manipulation.
 */
class Image {
    public:
        /**
         * @brief Constructor that loads an image from a file.
         * @param file_name Name of the image file to load.
         * @param allocator Optional pointer to a BuddyAllocator for memory management.
         */
        Image(const std::string &file_name, BuddyAllocator *allocator = nullptr);
        
        /**
         * @brief Constructor that loads an image from a memory buffer.
         * @param data Pointer to the image data.
         * @param high Image height.
         * @param width Image width.
         * @param channels Number of color channels.
         * @param allocator Optional pointer to a BuddyAllocator for memory management.
         */
        Image(unsigned char* data, int high, int width, int channels, BuddyAllocator* allocator = nullptr);
        
        /**
         * @brief Destructor that releases memory used by the image.
         */
        ~Image();

        // Getters
        int get_high() const;  ///< Gets the image height.
        int get_width() const; ///< Gets the image width.
        int get_channels() const; ///< Gets the number of color channels in the image.
        unsigned char* get_data() const; ///< Gets the image data in a linear buffer.
        unsigned char*** get_pixels() const; ///< Gets the image pixels in a matrix format.

        /**
         * @brief Saves the image to a file.
         * @param file_name Name of the output file.
         * @param buddy Boolean flag indicating whether the Buddy allocator should be used.
         * @param allocator Optional pointer to a BuddyAllocator.
         */
        void save_image(const std::string &file_name, bool buddy, BuddyAllocator* allocator = nullptr) const;

    private:
        int high, width, channels; ///< Image dimensions and number of channels.
        std::string file_name; ///< Image file name.
        unsigned char ***pixels; ///< 3D matrix of pixels (height × width × channels).
        BuddyAllocator *allocator; ///< Buddy memory allocator.

        /**
         * @brief Converts a data buffer into a pixel matrix.
         * @param buffer Pointer to the image data.
         */
        void convert_buffer_to_matrix(unsigned char* buffer);
};

#endif
