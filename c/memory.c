//Chapter 14 Challenge 3
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"

static uint8_t* gHeap = NULL;
static size_t gHeapSize = 0;
static size_t gOffset = 0;

static size_t align8(size_t n) {
    return (n + 7u) & ~7u;
}

bool allocator_init(size_t heapBytes) {
    if (gHeap != NULL || heapBytes == 0) return false;

    gHeap = (uint8_t*)malloc(heapBytes);
    if (gHeap == NULL) return false;

    gHeapSize = heapBytes;
    gOffset = 0;
    return true;
}

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    if (gHeap == NULL || gHeapSize == 0 || newSize == 0) return NULL;

    size_t wanted = align8(newSize);
    if (gOffset + wanted > gHeapSize) return NULL;

    void* out = gHeap + gOffset;
    gOffset += wanted;

    if (pointer != NULL) {
        size_t copyBytes = oldSize < newSize ? oldSize : newSize;
        memcpy(out, pointer, copyBytes);
    }

    return out;
}

void allocator_shutdown(void){
	//Empty on purpose, here so C doesn't complain it's missing	
}