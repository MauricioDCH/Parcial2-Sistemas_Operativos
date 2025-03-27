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

#include "utils.h"


/**
 * @brief Constructor implementation for the UtilsImage class.
 * 
 * This constructor does not perform any initialization as the class
 * only contains utility functions.
 */
UtilsImage::UtilsImage() {}


/**
 * @brief Displays initial image processing information.
 * 
 * Prints key details about the operation, including file paths, rotation angle,
 * scaling factor, and the memory allocation strategy used.
 * 
 * @param file_name Path of the input image.
 * @param exit_file_name Path of the output image.
 * @param use_buddy Boolean flag indicating whether the Buddy System is used for memory allocation.
 * @param angle Rotation angle in degrees.
 * @param scalar Scaling factor for resizing the image.
 */
void UtilsImage::initial_info(const std::string &file_name, const std::string &exit_file_name, bool use_buddy, int angle, float scalar) {
    cout << "\n|=============================================================================================" << endl;
    cout << "| WELCOME TO THE IMAGE PROCESSOR!" << endl;
    cout << "|=============================================================================================" << endl;
    cout << "| Initial information:"  << endl;
    cout << "|=============================================================================================" << endl;
    cout << "| Loading image from path: ----->| " << file_name << endl;
    cout << "| Saving image in the path: ---->| " << exit_file_name << endl;
    cout << "| Angle of rotation: ----------->| " << angle << "°" << endl;
    cout << "| Factor of scale: ------------->| 1:" << scalar << endl;
    cout << "| Using for memory allocation: ->| " << (use_buddy ? "Buddy System." : "new/delete.") << endl;
    cout << "|=============================================================================================" << endl;
}


/**
 * @brief Displays the correct usage of the program.
 * 
 * Prints instructions on how to execute the program, including the required and optional parameters.
 */
void UtilsImage::show_usage() {
    std::cout << "\n\nUsage: ./mdch_april <input_image> <output_image> -angle <0-360> -scalar <factor> [-buddy]\n";
    std::cout << "Example: ./mdch_april example_images/gatico.jpeg example_images/gatico_output.jpeg -angle 45 -scalar 1.5 -buddy\n";
    std::cout << "\nOptions:\n";
    std::cout << "  -angle <0-360>   -> Rotation angle in degrees.\n";
    std::cout << "  -scalar <factor> -> Scaling factor (can be less or greater than 1).\n";
    std::cout << "  -buddy           -> (Optional) Use the Buddy System for memory allocation.\n";
}


/**
 * @brief Retrieves the memory size of an image file.
 * 
 * This function extracts the image's width, height, and number of color channels
 * without fully loading the image into memory. It then calculates the total
 * memory size required to store the image.
 * 
 * @param file_name The path to the image file.
 * @return The size of the image in bytes, or 0 if the image could not be read.
 */
size_t UtilsImage::get_image_size(const std::string &file_name) {
    int width, height, channels;

    // Obtener información de la imagen sin cargarla en memoria
    if (!stbi_info(file_name.c_str(), &width, &height, &channels)) {
        std::cerr << "Error: No se pudo leer la información de la imagen '" << file_name << "'.\n";
        return 0;
    }

    // Calcular el tamaño en MB
    return static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels);
}


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
void UtilsImage::show_images_size(int original_image_width, int original_image_high, int scaled_image_width, int scaled_image_high, int rotated_image_width, int rotated_image_high, int channels){
    cout << "|=============================================================================================" << endl;
    cout << "\n|=============================================================================================" << endl;
    cout << "| Images size:" << endl;
    cout << "|=============================================================================================" << endl;
    cout << "| [INFO] Original image size: ------>| Channels: " << channels << " | Dimensions: " << original_image_width << " px x " << original_image_high << " px." << endl;
    cout << "| [INFO] Scaled image size: -------->| Channels: " << channels << " | Dimensions: " << scaled_image_width << " px x " << scaled_image_high << " px." << endl;
    cout << "| [INFO] Rotated image size: ------->| Channels: " << channels << " | Dimensions: " << rotated_image_width << " px x " << rotated_image_high << " px." << endl;
    cout << "|=============================================================================================" << endl;
}


/**
 * @brief Displays the total memory and time used in the execution.
 * 
 * This function prints the details such as the memory and time used in the execution.
 * 
 * @param time_usage Total time usage in the process.
 * @param memory_usage Total amount of memory usage in the process.
 * @param exit_file_name Exit file name.
 */
void UtilsImage::time_memory_usage(double time_usage, long memory_usage, const std::string &exit_file_name){
    cout << "\n|=============================================================================================" << endl;
    cout << "| Time and Memory used: " << endl;
    cout << "|=============================================================================================" << endl;
    cout << "| [INFO] Time in execution: ------------>| " << fixed << setprecision(2) << time_usage / 1000.0 << " seconds." << endl;
    cout << "| [INFO] Memory used in the execution: ->| " << memory_usage/1024 << " MB" << endl;
    cout << "| [INFO] Image successfully saved in: -->| '" << exit_file_name << "'.\n";
    cout << "|=============================================================================================" << endl;
    cout << "| THANKS FOR USE THE IMAGE PROCESSOR." << endl;
    cout << "|=============================================================================================" << endl;

}