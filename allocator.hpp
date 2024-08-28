#pragma once
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdint> // For uint8_t

namespace allocator {

    // A simple linear allocator that allocates memory sequentially.
    class LinearAllocator {
    public:
        // Constructor initializes allocator with null data
        LinearAllocator() = default;

        // Initializes the allocator with a memory block and its size
        void init(void* mem, size_t size) noexcept {
            data_ = static_cast<uint8_t*>(mem);
            capacity_ = size;
            reset();
        }

        // Allocates a block of memory of the requested size. Returns nullptr if out of space.
        uint8_t* allocate(size_t size) noexcept {
            if (offset_ + size > capacity_) {
                return nullptr; // Not enough memory
            }
            uint8_t* ptr = data_ + offset_;
            offset_ += size;
            return ptr;
        }

        // Frees a specified amount of memory by reducing the offset (doesn't actually deallocate memory)
        void free(size_t size) noexcept {
            size = std::min(size, offset_);
            offset_ -= size;
        }

        // Resets the allocator, making all memory available again
        void reset() noexcept {
            offset_ = 0;
        }

    private:
        uint8_t* data_ = nullptr; // Pointer to the beginning of the managed memory block
        size_t capacity_ = 0;     // Total size of the managed memory block
        size_t offset_ = 0;       // Current offset for the next allocation
    };

    // A block-based allocator that manages memory in blocks and uses a free list for reuse.
    template<typename T, size_t BlockSize = 256>
    class BlockAllocator {
    public:
        // Allocates an object of type T and constructs it with the provided arguments.
        template<typename... Args>
        T* allocate(Args&&... args) {
            if (free_list_.empty()) {
                // Allocate a new block when the free list is empty
                allocate_block();
            }

            T* ptr = free_list_.back();
            free_list_.pop_back();
            return new (ptr) T(std::forward<Args>(args)...); // Construct T in-place using placement new
        }

        // Frees an object and adds its memory back to the free list.
        void free(T* ptr) {
            if (ptr) {
                ptr->~T(); // Explicitly call the destructor
                free_list_.push_back(ptr); // Recycle the memory
            }
        }

        // Destructor to clean up all allocated blocks
        ~BlockAllocator() {
            for (auto& block : blocks_) {
                auto ptr = reinterpret_cast<T*>(block.data());
                for (size_t i = 0; i < BlockSize; ++i) {
                    ptr[i].~T(); // Destroy each element
                }
            }
        }

    private:
        // Allocates a new block and populates the free list with pointers to its elements
        void allocate_block() {
            blocks_.emplace_back(sizeof(T) * BlockSize);
            T* start_ptr = reinterpret_cast<T*>(blocks_.back().data());

            for (size_t i = 0; i < BlockSize; ++i) {
                free_list_.push_back(start_ptr + i);
            }
        }

        std::vector<std::vector<uint8_t>> blocks_; // Collection of memory blocks
        std::vector<T*> free_list_; // List of free elements ready for reuse
    };

} // namespace allocator
