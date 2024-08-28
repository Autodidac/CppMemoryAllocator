# CppMemoryAllocator
A Memory Allocator (header only) Single-File Library in C++20 for Game Design


# Custom Memory Allocators

This repository contains custom memory allocator implementations in C++ designed for high-performance applications where control over memory management is crucial. The allocators are particularly useful in game development, embedded systems, or any environment where minimizing memory allocation overhead is essential.

## Allocators Overview

### 1. `LinearAllocator`

A simple, fast memory allocator that allocates memory linearly from a pre-allocated block. It is ideal for scenarios where memory is allocated frequently but freed infrequently or all at once, such as in frame-based or temporary allocations.

- **Features**:
  - Very fast allocation.
  - Constant time complexity for `allocate` and `free`.
  - Memory is freed all at once by resetting the allocator.

- **Usage**:
  ```cpp
  allocator::LinearAllocator linearAllocator;
  void* memoryBlock = std::malloc(1024); // Allocate 1KB block
  linearAllocator.init(memoryBlock, 1024);

  uint8_t* data = linearAllocator.allocate(128); // Allocate 128 bytes
  // Use the allocated memory...
  linearAllocator.free(128); // Free 128 bytes (moves the offset back)
  linearAllocator.reset(); // Reset all allocations

2. BlockAllocator
A memory allocator that manages memory in blocks and uses a free list for efficient memory reuse. Suitable for allocating and deallocating objects frequently, such as in game engines or GUI systems.

Features:

Memory is divided into blocks for efficient reuse.
Suitable for objects of uniform size.
Low fragmentation and fast allocation.
Usage:
