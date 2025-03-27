/**
 * @file buddy_allocator.cpp
 * @author Mauricio David Correa Hernández.
 * @brief:  Implements the BuddyAllocator class for dynamic memory management.
 * @version 0.1
 * @date 2025-03-26
 * @copyright Copyright (c) 2025
 */


#include "buddy_allocator.h" // Include the header file


/**
 * @brief Constructor: assigns a memory block of the specified size.
 * @param size_full_memory The total size of the memory pool.
 */
BuddyAllocator::BuddyAllocator(size_t size_full_memory) {
    this->size_full_memory = size_full_memory;
    base_memory = std::malloc(size_full_memory);
    if (!base_memory) {
        cerr << "Error: Failed to allocate base memory with Buddy System.\n";
        exit(1);
    }

    // Initialize the structure with a single large block
    free_blocks[size_full_memory].push_back(base_memory);
}


/**
 * @brief Destructor: releases the reserved memory.
 */
BuddyAllocator::~BuddyAllocator() {
    std::free(base_memory);
}


/**
 * @brief Allocates memory of the given size using the Buddy System.
 * @param size The size of the memory block to allocate.
 * @return A pointer to the allocated memory, or nullptr if allocation fails.
 */
void* BuddyAllocator::alloc(size_t size) {
    // Find the smallest block that can be used
    auto it = free_blocks.lower_bound(size);
    if (it == free_blocks.end()) {
        cerr << "Error: Not enough memory to allocate " << size << " bytes.\n";
        return nullptr;
    }

    // Take the found block
    size_t blockSize = it->first;
    void* block = it->second.back();
    it->second.pop_back();
    if (it->second.empty()) {
        free_blocks.erase(it);
    }

    // Split the block if it is larger than necessary
    size_t currentSize = blockSize;
    while (currentSize > size) {
        currentSize /= 2;
        void* buddy = (char*)block + currentSize;
        free_blocks[currentSize].push_back(buddy);
    }

    return block;
}


/**
 * @brief Frees a previously allocated memory block.
 * @param ptr Pointer to the memory block to free.
 * @param size The size of the block being freed.
 */
void BuddyAllocator::free(void* ptr, size_t size) {
    if (!ptr) return;

    size_t currentSize = size;
    void* block = ptr;

    while (currentSize < size_full_memory) {
        uintptr_t block_addr = reinterpret_cast<uintptr_t>(block);
        uintptr_t buddy_addr = block_addr ^ currentSize;
        void* buddy = reinterpret_cast<void*>(buddy_addr);

        auto& blocksAtSize = free_blocks[currentSize];

        auto it = std::find(blocksAtSize.begin(), blocksAtSize.end(), buddy);
        if (it == blocksAtSize.end()) {
            blocksAtSize.push_back(block);
            return;
        }

        blocksAtSize.erase(it);
        block = (block_addr < buddy_addr) ? block : buddy;
        currentSize *= 2;
    }

    free_blocks[currentSize].push_back(block);
}
