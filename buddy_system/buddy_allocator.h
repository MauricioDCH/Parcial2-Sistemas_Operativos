/**
 * @file buddy_allocator.h
 * @author Mauricio David Correa Hernández.
 * @brief:  Definition of the BuddyAllocator class - The BuddyAllocator manages memory by splitting it into 
 * power-of-two sized blocks. When memory is allocated, the smallest suitable block is provided, and when 
 * freed, adjacent free blocks are merged back together.
 * @version 0.1
 * @date 2025-03-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H


#include <map>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <algorithm>


using namespace std;


/**
 * @class BuddyAllocator
 * @brief A memory allocator implementing the Buddy System algorithm.
 */
class BuddyAllocator {
    public:
        /**
         * @brief Constructs a BuddyAllocator with a given memory size.
         * @param size_full_memory The total memory size to manage.
         */
        BuddyAllocator(size_t size_full_memory);

        /**
         * @brief Destructor that releases allocated memory.
         */
        ~BuddyAllocator();

        /**
         * @brief Allocates a block of memory of the requested size.
         * @param size The number of bytes to allocate.
         * @return Pointer to the allocated memory block, or nullptr if insufficient memory.
         */
        void* alloc(size_t size);
        
        /**
         * @brief Frees a previously allocated block of memory.
         * @param ptr Pointer to the memory block to free.
         * @param size The size of the memory block to be freed.
         */
        void free(void* ptr, size_t size);

    private:
        size_t size_full_memory; ///< Total memory size managed by the allocator.
        void* base_memory; ///< Pointer to the base memory block.
        std::map<size_t, std::vector<void*>> free_blocks; ///< Map of free memory blocks, categorized by size.
};

#endif
