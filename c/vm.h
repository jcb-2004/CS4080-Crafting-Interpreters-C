#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

//Chapter 15 Challenge 3
typedef struct {
    Chunk* chunk;
    uint8_t* ip;

    Value* stack;
    Value* stackTop;
    int stackCapacity;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);
void push(Value value);
Value pop();

#endif