/**
 * @file measure_memorytime.cpp
 * @author Mauricio David Correa Hernández.
 * @brief:  Implements of the Measure Memory and Time class - Class to measure memory usage and execution time for image processing tasks.
 * @version 0.1
 * @date 2025-03-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "measure_memorytime.h" // Include the header file


/**
 * @brief Constructor for MeasureMemoryTime class.
 */
MeasureMemoryTime::MeasureMemoryTime(){}


/**
 * @brief Calculates the nearest power of 2 that is greater than or equal to the given image size.
 * This is necessary for the Buddy memory allocation method.
 * @param image_size The original image size.
 * @return The adjusted size, which is the nearest power of 2.
 */
size_t MeasureMemoryTime::get_buddy_size(size_t image_size){
    size_t buddy_size = 1;
    while (buddy_size < image_size) {
        buddy_size *= 2;
    }
    return buddy_size;
}


/**
 * @brief Measures memory and time usage when processing an image using the Buddy memory allocation system.
 * The process includes loading an image, scaling it, rotating it, and saving the output.
 * @param image_size The original size of the image in bytes.
 * @param file_name The input image file path.
 * @param scalar The scaling factor for the image.
 * @param buddy Boolean flag for using the Buddy memory allocator.
 * @param angle The angle (in degrees) for image rotation.
 * @param exit_file_name The output file path.
 * @return A MemoryTimeStruct containing the execution time and memory usage.
 */
MemoryTimeStruct MeasureMemoryTime::MesureMemoryAndTimeForBuddyAllocator(size_t image_size, const std::string &file_name, float scalar, bool buddy, int angle, const std::string &exit_file_name){
    // Initialization of the chronometer.
    UtilsImage utils;
    auto start = high_resolution_clock::now();
    cout << "\n|=============================================================================================" << endl;
    cout << "| Execution information:" << endl;
    cout << "|=============================================================================================" << endl;
    cout << "| [INFO] USING BUDDY SYSTEM FOR MEMORY ALLOCATION.\n|=================================================" << endl;
    cout << "| ----| SCALING SECTION. |----\n|" << endl;
    
    MemoryTimeStruct measureMemoryTimeStruct;

    // Adjust image size to the nearest power of 2 for Buddy allocation
    size_t buddy_size = get_buddy_size(image_size);
    BuddyAllocator allocador1(buddy_size);

    // Load the image into memory using the Buddy System
    Image img(file_name, &allocador1);
    ProcessImage processor(&img);

    // Scale the image
    auto start_chronometer_for_scaling  = high_resolution_clock::now();
    
    Image_info scaled_info = processor.image_scaling(img.get_data(), img.get_width(), img.get_high(), img.get_channels(), scalar, buddy, &allocador1);
    
    
    auto finish_chronometer_for_scaling  = high_resolution_clock::now();
    auto duracion_chronometer_for_scaling = duration_cast<milliseconds>(finish_chronometer_for_scaling - start_chronometer_for_scaling).count();

    cout << "| [INFO] Time taken to scale the image: " << fixed << setprecision(2) << duracion_chronometer_for_scaling/1000.0 << " seconds." << endl;

    // Measure the memory used in the scaling of the image.
    long memory_after_scaling = scaled_info.memory_used_for_process;
    long memory_used_scaling = memory_after_scaling - 0;

    // Verify successful scaling
    if (!scaled_info.new_data) {
        cerr << "[ERROR] Could not allocate memory for the scaled image." << endl;
        std::exit(EXIT_FAILURE);
    }

    // Adjust image size to the nearest power of 2 for Buddy allocation
    size_t image_size2 = scaled_info.new_width * scaled_info.new_high * img.get_channels();
    size_t buddy_size2 = get_buddy_size(image_size2);
    BuddyAllocator allocador2(buddy_size2);
    
    // Rotate the image
    cout << "|\n| ----| ROTATION SECTION.|----\n|" << endl;
    auto start_chronometer_for_rotation  = high_resolution_clock::now();
    Image_info rotated_info = processor.image_rotation(scaled_info.new_data, scaled_info.new_width, scaled_info.new_high, img.get_channels(), angle, buddy, &allocador2);
    auto finish_chronometer_for_rotation  = high_resolution_clock::now();
    auto duracion_chronometer_for_rotation = duration_cast<milliseconds>(finish_chronometer_for_rotation - start_chronometer_for_rotation).count();

    cout << "| [INFO] Time taken to rotate the image: " << fixed << setprecision(2) << duracion_chronometer_for_rotation/1000.0 << " seconds." << endl;

    // Measure the memory used in the rotation of the image.
    long memory_after_rotation = get_memory_usage();    
    long memory_used_rotation = memory_after_rotation - 0;

    // Verify successful rotation
    if (!rotated_info.new_data) {
        cerr << "[ERROR] Could not allocate memory for the rotated image." << endl;
        std::exit(EXIT_FAILURE);
    }

    // Image creation.
    cout << "|\n| ----| IMAGE CREATION SECTION.|----\n|" << endl;
    auto init_create_img = high_resolution_clock::now();
    Image rotated_img(rotated_info.new_data, rotated_info.new_high, rotated_info.new_width, img.get_channels(), &allocador2);
    auto finished_create_img = high_resolution_clock::now();
    auto duracion_create_img = duration_cast<milliseconds>(finished_create_img - init_create_img).count();

    cout << "| [INFO] Time taken to create the image: " <<  fixed << setprecision(2) << duracion_create_img/1000.0 << " seconds." << endl;
    
    // Adjust size to the nearest power of 2.
    size_t buddy_size3 = get_buddy_size(rotated_img.get_width() * rotated_img.get_high() * rotated_img.get_channels());
    BuddyAllocator allocador3(buddy_size3);

    // Save the rotated image.
    cout << "|\n| ----| IMAGE SAVE SECTION.|----\n|" << endl;
    auto init_save_img = high_resolution_clock::now();
    rotated_img.save_image(exit_file_name, buddy, &allocador3);
    auto finished_save_img = high_resolution_clock::now();
    auto duracion_save_img = duration_cast<milliseconds>(finished_save_img - init_save_img).count();
    cout << "| [INFO] Time taken to save rotated image: "  << fixed << setprecision(2) << duracion_save_img/1000.0 << " seconds." << endl;

    // Show images size
    utils.show_images_size( img.get_width(), img.get_high(), 
                            scaled_info.new_width, scaled_info.new_high, 
                            rotated_img.get_width(), rotated_img.get_high(),
                            img.get_channels());

    // Measure total time taken for the process
    auto finished = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(finished - start).count();
    
    // Measure total memory consumed for the process
    long memory_used = memory_used_scaling + memory_used_rotation;
    
    measureMemoryTimeStruct.time_used = duracion;
    measureMemoryTimeStruct.memory_usage = memory_used;
    return measureMemoryTimeStruct;
}

/**
 * @brief Measures memory and time usage using `new/delete` for memory allocation.
 * This follows the same steps as the Buddy allocator method but uses `new` and `delete` instead.
 * @param file_name The input image file path.
 * @param scalar The scaling factor for the image.
 * @param buddy Boolean flag for using the Buddy memory allocator.
 * @param angle The angle (in degrees) for image rotation.
 * @param exit_file_name The output file path.
 * @return A MemoryTimeStruct containing execution time and memory usage.
 */
MemoryTimeStruct MeasureMemoryTime::MesureMemoryAndTimeForNewDelete(const std::string &file_name,float scalar, bool buddy, int angle, const std::string &exit_file_name){
    // Initialization of the chronometer.
    UtilsImage utils;
    auto start = high_resolution_clock::now();
    cout << "\n|=============================================================================================" << endl;
    cout << "| Execution information:" << endl;
    cout << "|=============================================================================================" << endl;
    cout << "| [INFO] USING NEW/DELETE SYSTEM FOR MEMORY ALLOCATION.\n|======================================================" << endl;
    cout << "| ----| SCALING SECTION. |----\n|" << endl;

    MemoryTimeStruct measureMemoryTimeStruct;
    
    // Load the image into memory using the Buddy System
    Image img(file_name);
    ProcessImage processor(&img);
    
    // Scale the image
    auto start_chronometer_for_scaling  = high_resolution_clock::now();
    Image_info scaled_info = processor.image_scaling(img.get_data(), img.get_width(), img.get_high(), img.get_channels(), scalar, buddy);
    auto finish_chronometer_for_scaling  = high_resolution_clock::now();
    auto duracion_chronometer_for_scaling = duration_cast<milliseconds>(finish_chronometer_for_scaling - start_chronometer_for_scaling).count();

    cout << "| [INFO] Time taken to scale the image: " << fixed << setprecision(2) << duracion_chronometer_for_scaling/1000.0 << " seconds." << endl;

    // Measure the memory used in the scaling of the image.
    long memory_after_scaling = scaled_info.memory_used_for_process;
    long memory_used_scaling = memory_after_scaling - 0;

    // Rotate the image
    cout << "|\n| ----| ROTATION SECTION. |----\n|" << endl;
    auto start_chronometer_for_rotation  = high_resolution_clock::now();
    Image_info rotated_info = processor.image_rotation(scaled_info.new_data, scaled_info.new_width, scaled_info.new_high, img.get_channels(), angle, buddy);
    auto finish_chronometer_for_rotation  = high_resolution_clock::now();
    auto duracion_chronometer_for_rotation = duration_cast<milliseconds>(finish_chronometer_for_rotation - start_chronometer_for_rotation).count();

    cout << "| [INFO] Time taken to rotate the image: " << fixed << setprecision(2) << duracion_chronometer_for_rotation/1000.0 << " seconds." << endl;

    // Measure the memory used in the rotation of the image.
    long memory_after_rotation = get_memory_usage();    
    long memory_used_rotation = memory_after_rotation - 0;

    // Verify successful rotation
    if (!rotated_info.new_data) {
        cerr << "[ERROR] Could not allocate memory for the rotated image." << endl;
        std::exit(EXIT_FAILURE);
    }

    // Image creation.
    cout << "|\n| ----| IMAGE CREATION SECTION. |----\n|" << endl;
    auto init_create_img = high_resolution_clock::now();
    // Create a new image with the rotated data (without creating a new BuddyAllocator)
    Image rotated_img(rotated_info.new_data, rotated_info.new_high, rotated_info.new_width, img.get_channels());
    auto finished_create_img = high_resolution_clock::now();
    auto duracion_create_img = duration_cast<milliseconds>(finished_create_img - init_create_img).count();

    cout << "| [INFO] Time taken to create the image: " <<  fixed << setprecision(2) << duracion_create_img/1000.0 << " seconds." << endl;
    

    // Save the rotated image.
    cout << "|\n| ----| IMAGE SAVE SECTION. |----\n|" << endl;
    auto init_save_img = high_resolution_clock::now();
    rotated_img.save_image(exit_file_name, buddy);
    auto finished_save_img = high_resolution_clock::now();
    auto duracion_save_img = duration_cast<milliseconds>(finished_save_img - init_save_img).count();
    cout << "| [INFO] Time taken to save rotated image: "  << fixed << setprecision(2) << duracion_save_img/1000.0 << " seconds." << endl;

    // Show images size
    utils.show_images_size( img.get_width(), img.get_high(), 
                            scaled_info.new_width, scaled_info.new_high, 
                            rotated_img.get_width(), rotated_img.get_high(),
                            img.get_channels());
    
    // Measure total time taken for the process
    auto finished = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(finished - start).count();
    
    // Measure total memory consumed for the process
    long memory_used = memory_used_scaling + memory_used_rotation;
    
    measureMemoryTimeStruct.time_used = duracion;
    measureMemoryTimeStruct.memory_usage = memory_used;
    return measureMemoryTimeStruct;
}

/**
 * @brief Retrieves the current memory usage of the process.
 * @return The memory usage in kilobytes.
 */
long MeasureMemoryTime::get_memory_usage(){
    std::ifstream statm("/proc/self/statm");
    long size, resident;
    statm >> size >> resident;
    statm.close();
    return resident * sysconf(_SC_PAGESIZE) / 1024;
}