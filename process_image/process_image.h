/**
 * @file process_image.h
 * @author Mauricio David Correa Hernández.
 * @brief:  Definition of the Image class - Class that provides image processing operations such as scaling and rotation.
 * @version 0.1
 * @date 2025-03-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

#include <string>
#include <cstring>
#include <iostream>
#include <cmath>

#include "../image/image.h" // Include of image library
#include "../buddy_system/buddy_allocator.h"  // Include of buddy_allocator library
#include "../measure_memorytime/measure_memorytime.h" // Include of measure_memorytime library

using namespace std;

/**
 * @class ProcessImage
 * @brief A class that provides image processing operations such as scaling and rotation.
 */
class ProcessImage {
    public:
        /**
         * @brief Constructor to initialize an image.
         * @param img Pointer to an Image object.
         */
        ProcessImage(Image *img);

        /**
         * @brief Scales an image using bilinear interpolation.
         * @param image Pointer to the input image data.
         * @param width Width of the input image.
         * @param high Height of the input image.
         * @param channels Number of color channels in the image.
         * @param scalar Scaling factor.
         * @param buddy Flag to use the Buddy Allocator.
         * @param buddyAllocator Pointer to a BuddyAllocator (default is nullptr).
         * @return Image_info structure containing the scaled image and metadata.
         */
        Image_info image_scaling(const unsigned char* image, int width, int high, int channels, float scalar, bool buddy, BuddyAllocator* buddyAllocator = nullptr);
        
        /**
         * @brief Rotates an image by a given angle using bilinear interpolation.
         * @param image Pointer to the input image data.
         * @param width Width of the input image.
         * @param high Height of the input image.
         * @param channels Number of color channels in the image.
         * @param theta Rotation angle in degrees.
         * @param buddy Flag to use the Buddy Allocator.
         * @param buddyAllocator Pointer to a BuddyAllocator (default is nullptr).
         * @return Image_info structure containing the rotated image and metadata.
         */
        Image_info image_rotation(const unsigned char* image, int width, int high, int channels, float theta, bool buddy, BuddyAllocator* buddyAllocator = nullptr);

    private:
        Image *image; ///< Pointer to an Image object.
};

#endif
