/**
 * @file image.h
 * @author Mauricio David Correa Hernández.
 * @brief:  Implements of the Image class - Class representing an image and allowing its manipulation.
 * @version 0.1
 * @date 2025-03-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "image.h" // Include the header file


/**
 * @brief Constructor that loads an image from a file.
 * @param file_name Name of the image file to load.
 * @param allocator Optional pointer to a BuddyAllocator for memory management.
 */
Image::Image(const std::string &file_name, BuddyAllocator *allocator)
    : allocator(allocator) {

    unsigned char* buffer = stbi_load(file_name.c_str(), &width, &high, &channels, 0);
    if (!buffer) {
        cerr << "Error: Could not load image '" << file_name << "'.\n";
        exit(1);
    }

    convert_buffer_to_matrix(buffer);
    stbi_image_free(buffer);
}


/**
 * @brief Constructor that loads an image from a memory buffer.
 * @param data Pointer to the image data.
 * @param high Image height.
 * @param width Image width.
 * @param channels Number of color channels.
 * @param allocator Optional pointer to a BuddyAllocator for memory management.
 */
Image::Image(unsigned char* data, int high, int width, int channels, BuddyAllocator* allocator)
    : high(high), width(width), channels(channels), allocator(allocator) {
    
    if(!data) {
        cerr << "Error: Could not load image from buffer.\n";
        exit(1);
    }
    
    convert_buffer_to_matrix(data);
    stbi_image_free(data);
}


/**
 * @brief Destructor that releases memory used by the image.
 */
Image::~Image() {
    if (!allocator) {
        for (int position_in_y = 0; position_in_y < high; position_in_y++) {
            for (int position_in_x = 0; position_in_x < width; position_in_x++) {
                delete[] pixels[position_in_y][position_in_x];
            }
            delete[] pixels[position_in_y];
        }
        delete[] pixels;
    }
}


// Getters implementation
int Image::get_high() const { return high; } ///< Gets the image height.


int Image::get_width() const { return width; } ///< Gets the image width.


int Image::get_channels() const { return channels; } ///< Gets the number of color channels in the image.


unsigned char*** Image::get_pixels() const { return pixels; } ///< Gets the image pixels in a matrix format.


///< Gets the image data in a linear buffer.
unsigned char* Image::get_data() const {
    // Calculate the total buffer size
    int total_size = high * width * channels;
    
    // Allocate memory for the output buffer
    unsigned char* data_buffer = new unsigned char[total_size];

    int index = 0;
    for (int position_in_y = 0; position_in_y < high; position_in_y++) {
        for (int position_in_x = 0; position_in_x < width; position_in_x++) {
            for (int channel = 0; channel < channels; channel++) {
                data_buffer[index++] = pixels[position_in_y][position_in_x][channel];
            }
        }
    }

    return data_buffer;
}


/**
 * @brief Converts a data buffer into a pixel matrix.
 * @param buffer Pointer to the image data.
 */
void Image::convert_buffer_to_matrix(unsigned char* buffer) {
    int index = 0;
    pixels = new unsigned char**[high];

    for (int position_in_y = 0; position_in_y < high; position_in_y++) {
        pixels[position_in_y] = new unsigned char*[width];
        for (int position_in_x = 0; position_in_x < width; position_in_x++) {
            pixels[position_in_y][position_in_x] = new unsigned char[channels];
            for (int channel = 0; channel < channels; channel++) {
                pixels[position_in_y][position_in_x][channel] = buffer[index++];
            }
        }
    }
}


/**
 * @brief Saves the image to a file.
 * @param file_name Name of the output file.
 * @param buddy Boolean Indicates whether to use BuddyAllocator for memory.
 * @param allocator Optional pointer to a BuddyAllocator.
 */
void Image::save_image(const std::string &file_name, bool buddy, BuddyAllocator* allocator) const {
    size_t buffer_size = high * width * channels;
    unsigned char* buffer = nullptr;

    if (buddy) {
        if (!allocator) {  // Check that allocator is not null
            cerr << "Error: BuddyAllocator not initialized.\n";
            exit(1);
        }

        buffer = static_cast<unsigned char*>(allocator->alloc(buffer_size));
        if (!buffer) {
            cerr << "Error: Memory allocation with BuddyAllocator failed.\n";
            exit(1);
        }
    } else {
        buffer = new unsigned char[buffer_size];
    }

    int index = 0;
    for (int position_in_y = 0; position_in_y < high; position_in_y++) {
        for (int position_in_x = 0; position_in_x < width; position_in_x++) {
            for (int channel = 0; channel < channels; channel++) {
                buffer[index++] = pixels[position_in_y][position_in_x][channel];
            }
        }
    }

    if (!stbi_write_png(file_name.c_str(), width, high, channels, buffer, width * channels)) {
        cerr << "Error: Could not save image '" << file_name << "'.\n";
        if (buddy) {
            allocator->free(buffer, buffer_size);
        } else {
            delete[] buffer;
        }
        exit(1);
    }

    std::cout << "| [INFO] Image successfully saved.'"<< "'.\n";

    if (buddy) {
        allocator->free(buffer, buffer_size);
    } else {
        delete[] buffer;
    }
}
