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
```cpp
allocator::BlockAllocator<MyClass> blockAllocator;
MyClass* object = blockAllocator.allocate(); // Allocate a new object
// Use the allocated object...
blockAllocator.free(object); // Free the object (adds it back to the free list)
```

Building and Integrating
To integrate these allocators into your project:

Include the LinearAllocator and BlockAllocator headers in your source files.
Ensure your build system is configured to compile the allocators.
```cpp
#include "LinearAllocator.h"
#include "BlockAllocator.h"
```
Requirements
C++17 or higher
A compatible C++ compiler (e.g., GCC, Clang, MSVC)
Contributing
Feel free to submit issues or pull requests for improvements or bug fixes. Contributions are always welcome!

License
This project is licensed under the MIT License - see the LICENSE file for details.


### Final Notes

This version of the memory allocators is clean, modern, and adheres to best practices in C++ programming. The `README.md` provides a clear guide for potential users or contributors on GitHub, detailing the purpose, usage, and how to integrate the allocators into other projects.
