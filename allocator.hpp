// allocator.hpp

/* 
This software is dual-licensed to the public domain and under the following
license: you are granted a perpetual, irrevocable license to copy, modify,
publish, and distribute this file as you see fit.
*/

#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <cstdlib>   // Standard library header for memory functions
#include <cassert>   // Standard library header for assertions
#include <algorithm> // For std::min
#include <span>      // For std::span (C++20)
#include <concepts>  // For concepts (C++20)

// Memory management macros for user-defined allocators
#ifndef ALLOCATOR_ALLOC
#define ALLOCATOR_ALLOC(size) std::malloc(size) // Default to malloc
#endif

#ifndef ALLOCATOR_FREE
#define ALLOCATOR_FREE(ptr) std::free(ptr) // Default to free
#endif

namespace allocator {

// Linear Allocator Class
class LinearAllocator {
    uint8_t* data = nullptr;
    size_t capacity = 0;
    size_t offset = 0;

public:
    // Initialize allocator with memory and size
    constexpr void init(void* mem, size_t size) noexcept {
        data = static_cast<uint8_t*>(mem);
        capacity = size;
        reset();
    }

    // Allocate memory from the allocator
    [[nodiscard]] constexpr uint8_t* allocate(size_t size) noexcept {
        if (offset + size > capacity) {
            return nullptr; // Not enough space
        }
        uint8_t* ptr = data + offset;
        offset += size;
        return ptr;
    }

    // Free memory by adjusting the offset
    constexpr void free(size_t size) noexcept {
        size = std::min(size, offset);
        offset -= size;
    }

    // Reset the allocator to reuse memory
    constexpr void reset() noexcept {
        offset = 0;
    }
};

// Concept to ensure T is constructible
template<typename T>
concept Constructible = std::constructible_from<T>;

// Block Allocator Template Class
template<Constructible T, size_t block_size = 256>
class BlockAllocator {
    struct Block {
        std::span<uint8_t> mem;

        // Constructor to allocate memory for the block
        Block() {
            mem = std::span<uint8_t>(static_cast<uint8_t*>(ALLOCATOR_ALLOC(sizeof(T) * block_size)), sizeof(T) * block_size);
        }

        // Destructor to free allocated memory
        ~Block() {
            if (!mem.empty()) {
                ALLOCATOR_FREE(mem.data());
            }
        }
    };

    std::vector<Block> blocks;     // Vector to manage blocks of memory
    std::vector<T*> free_list;     // Vector to manage the free list of T*

public:
    // Allocate an object of type T
    template<typename... Args>
    [[nodiscard]] T* allocate(Args&&... args) {
        if (free_list.empty()) {
            Block& block = blocks.emplace_back();
            T* ptr = reinterpret_cast<T*>(block.mem.data());
            for (size_t i = 0; i < block_size; ++i) {
                free_list.push_back(ptr + i);
            }
        }
        T* ptr = free_list.back();
        free_list.pop_back();
        return new (ptr) T(std::forward<Args>(args)...); // Construct in-place
    }

    // Free an object of type T
    void free(T* ptr) {
        ptr->~T(); // Explicitly call the destructor
        free_list.push_back(ptr);
    }

    // Destructor to clean up all blocks
    ~BlockAllocator() {
        for (auto& block : blocks) {
            for (size_t i = 0; i < block_size; ++i) {
                T* ptr = reinterpret_cast<T*>(block.mem.data()) + i;
                ptr->~T();
            }
        }
    }
};

} // namespace allocator

#endif // ALLOCATOR_HPP
