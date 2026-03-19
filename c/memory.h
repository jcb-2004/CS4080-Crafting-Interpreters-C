//Chapter 14 Challenge 3
#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*)reallocate((pointer), sizeof(type) * (oldCount), \
                              sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate((pointer), sizeof(type) * (oldCount), 0)

bool allocator_init(size_t heapBytes);
void allocator_shutdown(void);
void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif