#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdint> // For uint8_t

namespace allocator {

    // A simple linear allocator that allocates memory sequentially.
    struct LinearAllocator {
        uint8_t* data = nullptr; // Pointer to the beginning of the managed memory block
        size_t capacity = 0;     // Total size of the managed memory block
        size_t offset = 0;       // Current offset for the next allocation

        // Initializes the allocator with a memory block and its size
        constexpr void init(void* mem, size_t size) {
            data = static_cast<uint8_t*>(mem);
            capacity = size;
            reset();
        }

        // Allocates a block of memory of the requested size. Returns nullptr if out of space.
        constexpr uint8_t* allocate(size_t size) {
            if (offset + size > capacity) {
                return nullptr; // Not enough memory
            }
            uint8_t* ptr = data + offset;
            offset += size;
            return ptr;
        }

        // Frees a specified amount of memory by reducing the offset (doesn't actually deallocate memory)
        constexpr void free(size_t size) {
            size = std::min(size, offset);
            offset -= size;
        }

        // Resets the allocator, making all memory available again
        constexpr void reset() {
            offset = 0;
        }
    };

    // A block-based allocator that manages memory in blocks and uses a free list for reuse.
    template<typename T, size_t BlockSize = 256>
    struct BlockAllocator {
        struct Block {
            std::vector<uint8_t> memory; // Raw memory block for storing elements
        };

        std::vector<Block> blocks;    // Collection of memory blocks
        std::vector<T*> free_list;    // List of free elements ready for reuse

        // Allocates an object of type T and constructs it with the provided arguments.
        template<typename... Args>
        T* allocate(Args&&... args) {
            if (free_list.empty()) {
                // Allocate a new block when the free list is empty
                allocate_block();
            }

            T* ptr = free_list.back();
            free_list.pop_back();
            return new (ptr) T(std::forward<Args>(args)...); // Construct T in-place using placement new
        }

        // Frees an object and adds its memory back to the free list.
        void free(T* ptr) {
            if (ptr) {
                ptr->~T(); // Explicitly call the destructor
                free_list.push_back(ptr); // Recycle the memory
            }
        }

    private:
        // Allocates a new block and populates the free list with pointers to its elements
        void allocate_block() {
            free_list.reserve(BlockSize);
            Block& block = blocks.emplace_back();
            block.memory.resize(sizeof(T) * BlockSize);
            T* start_ptr = reinterpret_cast<T*>(block.memory.data());

            for (size_t i = 0; i < BlockSize; ++i) {
                free_list.push_back(start_ptr + i);
            }
        }
    };

} // namespace allocator
