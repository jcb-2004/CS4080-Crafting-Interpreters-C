#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void initTable(Table* table) {
  table->count = 0;
  table->capacity = 0;
  table->entries = NULL;
}

void freeTable(Table* table) {
  FREE_ARRAY(Entry, table->entries, table->capacity);
  initTable(table);
}

//Chapter 20 Challenge 1
static uint32_t hashValue(Value value) {
  switch (value.type) {
    case VAL_BOOL:
      return AS_BOOL(value) ? 1 : 0;
    case VAL_NIL:
      return 2;
    case VAL_NUMBER: {
      double num = AS_NUMBER(value);
      uint64_t bits;
      memcpy(&bits, &num, sizeof(bits));
      return (uint32_t)(bits ^ (bits >> 32));
    }

    case VAL_OBJ:
      if (IS_STRING(value)) return AS_STRING(value)->hash;
      return 0;
  }

  return 0;
}

static Entry* findEntry(Entry* entries, int capacity,
                        Value key) { //Chapter 20 Challenge 1
  uint32_t index = hashValue(key) % capacity; //Chapter 20 Challenge 1
  Entry* tombstone = NULL;
	
  for (;;) {
    Entry* entry = &entries[index];
    if (IS_NIL(entry->key)) { //Chapter 20 Challenge 1
      if (IS_NIL(entry->value)) {
        // Empty entry.
        return tombstone != NULL ? tombstone : entry;
      } else {
        // We found a tombstone.
        if (tombstone == NULL) tombstone = entry;
      }
    } else if (valuesEqual(entry->key, key)) { //Chapter 20 Challenge 1
      // We found the key.
      return entry;
    }
	  
    index = (index + 1) % capacity;
  }
}

bool tableGet(Table* table, Value key, Value* value) { //Chapter 20 Challenge 1
  if (table->count == 0) return false;

  Entry* entry = findEntry(table->entries, table->capacity, key);
  if (IS_NIL(entry->key)) return false; //Chapter 20 Challenge 1

  *value = entry->value;
  return true;
}

static void adjustCapacity(Table* table, int capacity) {
  Entry* entries = ALLOCATE(Entry, capacity);
  for (int i = 0; i < capacity; i++) {
    entries[i].key = NIL_VAL; //Chapter 20 Challenge 1
    entries[i].value = NIL_VAL;
  }

  table->count = 0;
  for (int i = 0; i < table->capacity; i++) {
    Entry* entry = &table->entries[i];
    if (IS_NIL(entry->key)) continue; //Chapter 20 Challenge 1

    Entry* dest = findEntry(entries, capacity, entry->key);
    dest->key = entry->key;
    dest->value = entry->value;
    table->count++;
  }
	
  FREE_ARRAY(Entry, table->entries, table->capacity);
  table->entries = entries;
  table->capacity = capacity;
}

bool tableSet(Table* table, Value key, Value value) { //Chapter 20 Challenge 1
  if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
    int capacity = GROW_CAPACITY(table->capacity);
    adjustCapacity(table, capacity);
  }
	
  Entry* entry = findEntry(table->entries, table->capacity, key);
  bool isNewKey = IS_NIL(entry->key); //Chapter 20 Challenge 1
  if (isNewKey && IS_NIL(entry->value)) table->count++;

  entry->key = key;
  entry->value = value;
  return isNewKey;
}

bool tableDelete(Table* table, Value key) {
  if (table->count == 0) return false;

  // Find the entry.
  Entry* entry = findEntry(table->entries, table->capacity, key);
  if (IS_NIL(entry->key)) return false; //Chapter 20 Challenge 1

  // Place a tombstone in the entry.
  entry->key = NIL_VAL; //Chapter 20 Challenge 1
  entry->value = BOOL_VAL(true);
  return true;
}

void tableAddAll(Table* from, Table* to) {
  for (int i = 0; i < from->capacity; i++) {
    Entry* entry = &from->entries[i];
    if (!IS_NIL(entry->key)) { //Chapter 20 Challenge 1
      tableSet(to, entry->key, entry->value);
    }
  }
}

ObjString* tableFindString(Table* table, const char* chars,
                           int length, uint32_t hash) {
  if (table->count == 0) return NULL;

  uint32_t index = hash % table->capacity;
  for (;;) {
    Entry* entry = &table->entries[index];
    if (IS_NIL(entry->key)) { //Chapter 20 Challenge 1
      // Stop if we find an empty non-tombstone entry.
      if (IS_NIL(entry->value)) return NULL;
	//Chapter 20 Challenge 1
	} else if (IS_STRING(entry->key)) {
	  ObjString* key = AS_STRING(entry->key);
	  if (key->length == length &&
		  key->hash == hash &&
		  memcmp(key->chars, chars, length) == 0) {
		// We found it.
		return key;
	  }
	}

    index = (index + 1) % table->capacity;
  }
}