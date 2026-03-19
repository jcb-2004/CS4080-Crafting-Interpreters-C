#include "common.h"
#include "chunk.h"
#include "debug.h"
//Chapter 14 Challenge 3
#include "memory.h"
#include <stdio.h>

int main(int argc, const char* argv[]) {
  //Chapter 14 Challenge 3
  if (!allocator_init(1024 * 1024)) {
    fprintf(stderr, "Failed to initialize allocator.\n");
    return 1;
  }

  Chunk chunk;
  initChunk(&chunk);

  int constant = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);
  writeChunk(&chunk, OP_RETURN, 123);

  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
  allocator_shutdown();
  return 0;
}