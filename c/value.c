#include <stdio.h>
#include <string.h>

#include "object.h"
#include "memory.h"
#include "value.h"

void initValueArray(ValueArray* array) {
  array->values = NULL;
  array->capacity = 0;
  array->count = 0;
}

void writeValueArray(ValueArray* array, Value value) {
  if (array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->values = GROW_ARRAY(Value, array->values,
                               oldCapacity, array->capacity);
  }

  array->values[array->count] = value;
  array->count++;
}

void freeValueArray(ValueArray* array) {
  FREE_ARRAY(Value, array->values, array->capacity);
  initValueArray(array);
}

void printValue(Value value) {
#ifdef NAN_BOXING
  if (IS_BOOL(value)) {
    printf(AS_BOOL(value) ? "true" : "false");
  } else if (IS_NIL(value)) {
    printf("nil");
  } else if (IS_NUMBER(value)) {
    printf("%g", AS_NUMBER(value));
  } else if (IS_OBJ(value)) {
    printObject(value);
  }
#else
  switch (value.type) {
    case VAL_BOOL:
      printf(AS_BOOL(value) ? "true" : "false");
      break;
    case VAL_NIL: printf("nil"); break;
    case VAL_NUMBER: printf("%g", AS_NUMBER(value)); break;
    case VAL_OBJ: printObject(value); break;
	//Chapter 30 Challenge 2
    case VAL_SHORT_STRING:
      fwrite(
          AS_SHORT_STRING(value).chars,
          sizeof(char),
          AS_SHORT_STRING(value).length,
          stdout);
      break;
  }
#endif
}

//Chapter 30 Challenge 2
Value shortStringValue(const char* chars, int length) {
  Value value;
  value.type = VAL_SHORT_STRING;
  value.as.shortString.length = (uint8_t)length;

  for (int i = 0; i < SHORT_STRING_MAX; i++) {
    value.as.shortString.chars[i] = '\0';
  }

  memcpy(value.as.shortString.chars, chars, length);
  return value;
}

bool valuesEqual(Value a, Value b) {
#ifdef NAN_BOXING
  if (IS_NUMBER(a) && IS_NUMBER(b)) {
    return AS_NUMBER(a) == AS_NUMBER(b);
  }
  return a == b;
#else
  //Chapter 30 Challenge 2
  if (IS_STRING(a) && IS_STRING(b)) {
    int aLength = stringValueLength(&a);
    int bLength = stringValueLength(&b);

    if (aLength != bLength) return false;

    return memcmp(stringValueChars(&a), stringValueChars(&b), aLength) == 0;
  }
	
  if (a.type != b.type) return false;
  switch (a.type) {
    case VAL_BOOL:   return AS_BOOL(a) == AS_BOOL(b);
    case VAL_NIL:    return true;
    case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
    case VAL_OBJ:    return AS_OBJ(a) == AS_OBJ(b);
	case VAL_SHORT_STRING: return false; //Chapter 30 Challenge 2
    default:         return false; // Unreachable.
  }
#endif
}