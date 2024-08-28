# **CppMemoryAllocator**

A **header-only** single-file memory allocator library in **C++20** designed for game development and other high-performance applications where control over memory management is crucial.

## **Overview**

This repository contains custom memory allocator implementations in C++20 designed to provide fine-grained control over memory management. The allocators are particularly useful in game development, embedded systems, or any environment where minimizing memory allocation overhead is essential.

## **Allocators Overview**

### **1. `LinearAllocator`**

A simple, fast memory allocator that allocates memory linearly from a pre-allocated block. It is ideal for scenarios where memory is allocated frequently but freed infrequently or all at once, such as in frame-based or temporary allocations.

- **Features**:
  - Very fast allocation.
  - Constant time complexity for `allocate` and `free`.
  - Memory is freed all at once by resetting the allocator.

- **Usage**:

  ```cpp
  #define ALLOCATOR_IMPLEMENTATION
  #include "allocator.hpp"

  allocator::LinearAllocator linearAllocator;
  void* memoryBlock = std::malloc(1024); // Allocate 1KB block
  linearAllocator.init(memoryBlock, 1024);

  uint8_t* data = linearAllocator.allocate(128); // Allocate 128 bytes
  // Use the allocated memory...
  linearAllocator.free(128); // Free 128 bytes (moves the offset back)
  linearAllocator.reset(); // Reset all allocations
  std::free(memoryBlock); // Free the 1KB block when done


2. BlockAllocator
A memory allocator that manages memory in blocks and uses a free list for efficient memory reuse. Suitable for allocating and deallocating objects frequently, such as in game engines or GUI systems.

Features:

Memory is divided into blocks for efficient reuse.
Suitable for objects of uniform size.
Low fragmentation and fast allocation.
Usage: 
```cpp
#define ALLOCATOR_IMPLEMENTATION
#include "allocator.hpp"

class MyClass {
    // Define the class here
};

allocator::BlockAllocator<MyClass> blockAllocator;
MyClass* object = blockAllocator.allocate(); // Allocate a new object
// Use the allocated object...
blockAllocator.free(object); // Free the object (adds it back to the free list)
```
Building and Integrating
To integrate these allocators into your project:

Include the header-only library:

Ensure you include the allocator.hpp header in your source files:
```cpp
#define ALLOCATOR_IMPLEMENTATION
#include "allocator.hpp"
```
Set up your build system:

No special build configuration is required since this is a single-file header-only library. Make sure your compiler supports C++20.

Requirements
C++20 or higher: The library utilizes modern C++20 features like constexpr, concepts, and std::span.
A compatible C++ compiler: Ensure you use a compiler that supports C++20, such as GCC 10+, Clang 10+, or MSVC 2019+.
Contributing
Contributions are welcome! Feel free to submit issues or pull requests for improvements or bug fixes.

License
This project is dual-licensed to the public domain and under the following license: you are granted a perpetual, irrevocable license to copy, modify, publish, and distribute this file as you see fit.
