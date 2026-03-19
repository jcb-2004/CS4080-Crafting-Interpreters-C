#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"

VM vm; 

static void resetStack() {
  vm.stackTop = vm.stack;
}

//Chapter 15 Challenge 3
void initVM() {
    vm.stackCapacity = 256;
    vm.stack = malloc(sizeof(Value) * vm.stackCapacity);
    resetStack();
}

//Chapter 15 Challenge 3
void freeVM() {
	free(vm.stack);
}

//Chapter 15 Challenge 3
void push(Value value) {
    if (vm.stackTop - vm.stack >= vm.stackCapacity) {
        vm.stackCapacity *= 2;
        vm.stack = realloc(vm.stack, sizeof(Value) * vm.stackCapacity);
        vm.stackTop = vm.stack + vm.stackCapacity / 2;
    }

    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
//Chapter 15 Challenge 4
#define BINARY_OP(op) \
    do { \
        vm.stackTop[-2] = vm.stackTop[-2] op vm.stackTop[-1]; \
        vm.stackTop--; \
    } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
    disassembleInstruction(vm.chunk,
                           (int)(vm.ip - vm.chunk->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        push(constant);
        break;
      }
      case OP_ADD:      BINARY_OP(+); break;
      case OP_SUBTRACT: BINARY_OP(-); break;
      case OP_MULTIPLY: BINARY_OP(*); break;
      case OP_DIVIDE:   BINARY_OP(/); break;
      case OP_NEGATE:   vm.stackTop[-1] = -vm.stackTop[-1]; break; //Chapter 15 Challenge 4
      case OP_RETURN: {
        printValue(pop());
        printf("\n");
        return INTERPRET_OK;
      }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(Chunk* chunk) {
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run();
}