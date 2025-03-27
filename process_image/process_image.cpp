/**
 * @file process_image.cpp
 * @author Mauricio David Correa Hernández.
 * @brief:  Implements of the Image class - Class that provides image processing operations such as scaling and rotation.
 * @version 0.1
 * @date 2025-03-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "process_image.h" // Include the header file


/**
 * @brief Constructor to initialize an image.
 * @param img Pointer to an Image object.
 */
ProcessImage::ProcessImage(Image *img) : image(img) {}


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
Image_info ProcessImage::image_scaling(const unsigned char* image, int width, int high, int channels, float scalar, bool buddy, BuddyAllocator* buddyAllocator) {
    Image_info scaled_info;
    MeasureMemoryTime measureMemoryTime;

    int new_width, new_high;
    float scala_x, scala_y;
    
    // Calculate new dimensions
    new_width = scalar * width;
    new_high = scalar * high;
    scala_x = (float)width / new_width;
    scala_y = (float)high / new_high;
    
    scaled_info.new_width = new_width;
    scaled_info.new_high = new_high;
    
    cout << "| [INFO] Scaling factor on the x-axis: " << scala_x << endl;
    cout << "| [INFO] Scaling factor on the y-axis: " << scala_y << endl;
    
    // Create new image buffer using BuddyAllocator if needed
    size_t image_size = new_width * new_high * channels;
    unsigned char* scaled_image = nullptr;
    cout << "| [INFO] Scaled image size: " << (image_size / (1024 * 1024)) << " MB" << endl;

    if (buddy) {
        buddyAllocator = new BuddyAllocator(image_size);
        scaled_image = static_cast<unsigned char*>(buddyAllocator->alloc(image_size));
    
        if (!scaled_image) {
            cerr << "Error: Could not allocate memory with BuddyAllocator for Scaled Image.\n";
            exit(1);
        }
    } else {
        scaled_image = new unsigned char[image_size];
    }
    
    long memory_for_scaling = measureMemoryTime.get_memory_usage();
    long memory_for_loop = 0;

    // Perform bilinear interpolation
    for (int y = 0; y < new_high; y++) {
        for (int x = 0; x < new_width; x++) {
            float src_x = x * scala_x;
            float src_y = y * scala_y;

            int x0 = (int)src_x;
            int y0 = (int)src_y;
            int x1 = std::min(x0 + 1, width - 1);
            int y1 = std::min(y0 + 1, high - 1);

            float delta_x = src_x - x0;
            float delta_y = src_y - y0;

            for (int channel = 0; channel < channels; channel++) {
                unsigned char neighboring_pixel_00 = image[(y0 * width + x0) * channels + channel];
                unsigned char neighboring_pixel_10 = image[(y0 * width + x1) * channels + channel];
                unsigned char neighboring_pixel_01 = image[(y1 * width + x0) * channels + channel];
                unsigned char neighboring_pixel_11 = image[(y1 * width + x1) * channels + channel];

                float value =   (1 - delta_x) * (1 - delta_y) * neighboring_pixel_00 +
                                delta_x * (1 - delta_y) * neighboring_pixel_10 +
                                (1 - delta_x) * delta_y * neighboring_pixel_01 +
                                delta_x * delta_y * neighboring_pixel_11;

                scaled_image[(y * new_width + x) * channels + channel] = (unsigned char)value;
            }
        }
    }

    // Memory usage in each iteration
    memory_for_loop = measureMemoryTime.get_memory_usage();
    if (memory_for_scaling < memory_for_loop){
        memory_for_scaling = memory_for_loop;
    }
    
    scaled_info.memory_used_for_process = memory_for_scaling;
    scaled_info.new_data = scaled_image;
    
    return scaled_info;
}


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
Image_info ProcessImage::image_rotation(const unsigned char* image, int width, int high, int channels, float theta, bool buddy, BuddyAllocator* buddyAllocator) {
    Image_info rotated_info;
    MeasureMemoryTime measureMemoryTime;

    // Convert degrees to radians
    float radians = theta * M_PI / 180.0;

    // Calcular el centro de la imagen original
    int x_center = width / 2;
    int y_center = high / 2;

    // Calculate new dimensions of the rotated image
    int new_width = std::abs(width * cos(radians)) + std::abs(high * sin(radians));
    int new_high = std::abs(width * sin(radians)) + std::abs(high * cos(radians));

    rotated_info.new_width = new_width;
    rotated_info.new_high = new_high;

    int new_x_center = new_width / 2;
    int new_y_center = new_high / 2;

    // Create new image buffer with Buddy if needed
    size_t image_size = new_width * new_high * channels;
    unsigned char* rotated_image = nullptr;
    cout << "| [INFO] Rotated image size: " << (image_size / (1024 * 1024)) << " MB" << endl;

    if (buddy) {
        buddyAllocator = new BuddyAllocator(image_size);
        rotated_image = static_cast<unsigned char*>(buddyAllocator->alloc(image_size));
    
        if (!rotated_image) {
            cerr << "Error: Could not allocate memory with BuddyAllocator for Rotated Image.\n";
            exit(1);
        }
    } else {
        rotated_image = new unsigned char[image_size];
    }

    long memory_for_rotation = measureMemoryTime.get_memory_usage();
    long memory_for_loop = 0;

    // Initialize rotated image with black (0) or transparent (0 if it has alpha channel)
    for (size_t i = 0; i < image_size; i++)
        rotated_image[i] = (channels == 4) ? 0 : 0;

    // Apply inverse transformation with bilinear interpolation
    for (int y_position = 0; y_position < new_high; y_position++) {
        for (int x_position = 0; x_position < new_width; x_position++) {
            float x_original = (x_position - new_x_center) * cos(radians) - (y_position - new_y_center) * sin(radians) + x_center;
            float y_original = (x_position - new_x_center) * sin(radians) + (y_position - new_y_center) * cos(radians) + y_center;

            int x0 = (int)x_original;
            int y0 = (int)y_original;

            if (x0 >= 0 && x0 < width - 1 && y0 >= 0 && y0 < high - 1) {
                int x1 = std::min(x0 + 1, width - 1);
                int y1 = std::min(y0 + 1, high - 1);

                float delta_x = x_original - x0;
                float delta_y = y_original - y0;

                for (int channel = 0; channel < channels; channel++) {
                    unsigned char neighboring_pixel_00 = image[(y0 * width + x0) * channels + channel];
                    unsigned char neighboring_pixel_10 = image[(y0 * width + x1) * channels + channel];
                    unsigned char neighboring_pixel_01 = image[(y1 * width + x0) * channels + channel];
                    unsigned char neighboring_pixel_11 = image[(y1 * width + x1) * channels + channel];

                    float value =   (1 - delta_x) * (1 - delta_y) * neighboring_pixel_00 +
                                    delta_x * (1 - delta_y) * neighboring_pixel_10 +
                                    (1 - delta_x) * delta_y * neighboring_pixel_01 +
                                    delta_x * delta_y * neighboring_pixel_11;

                    rotated_image[(y_position * new_width + x_position) * channels + channel] = (unsigned char)value;
                }
            }
        }
        // Memory usage in each iteration
        memory_for_loop = measureMemoryTime.get_memory_usage();
        if (memory_for_rotation < memory_for_loop) {
            memory_for_rotation = memory_for_loop;
        }
    }

    rotated_info.memory_used_for_process = memory_for_rotation;
    rotated_info.new_data = rotated_image;

    return rotated_info;
}
