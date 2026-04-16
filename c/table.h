#ifndef clox_table_h
#define clox_table_h

#include "common.h"
#include "value.h"

typedef struct {
  Value key; //Chapter 20 Challenge 1
  Value value;
} Entry;

typedef struct {
  int count;
  int capacity;
  Entry* entries;
} Table;

void initTable(Table* table);
void freeTable(Table* table);
bool tableGet(Table* table, Value key, Value* value); //Chapter 20 Challenge 1
bool tableSet(Table* table, Value key, Value value); //Chapter 20 Challenge 1
bool tableDelete(Table* table, Value key); //Chapter 20 Challenge 1
void tableAddAll(Table* from, Table* to);
ObjString* tableFindString(Table* table, const char* chars,
                           int length, uint32_t hash);

#endif