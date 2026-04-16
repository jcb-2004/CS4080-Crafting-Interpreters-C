//Chapter 20 Challenge 3

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

static double nowSeconds(void) {
  return (double)clock() / CLOCKS_PER_SEC;
}

static ObjString* makeKey(const char* prefix, int i) {
  char buffer[64];
  int length = snprintf(buffer, sizeof(buffer), "%s%d", prefix, i);
  return copyString(buffer, length);
}

static ObjString** makeKeyArray(const char* prefix, int count) {
  ObjString** keys = (ObjString**)malloc(sizeof(ObjString*) * count);
  if (keys == NULL) {
    fprintf(stderr, "Out of memory allocating keys.\n");
    exit(1);
  }

  for (int i = 0; i < count; i++) {
    keys[i] = makeKey(prefix, i);
  }

  return keys;
}

static void benchInsert(int count) {
  Table table;
  initTable(&table);

  ObjString** keys = makeKeyArray("insertKey", count);

  double start = nowSeconds();

  for (int i = 0; i < count; i++) {
    tableSet(&table, keys[i], NUMBER_VAL(i));
  }

  double end = nowSeconds();

  printf("insert %d keys: %.6f sec\n", count, end - start);

  free(keys);
  freeTable(&table);
}

static void benchLookup(int count, int rounds) {
  Table table;
  initTable(&table);

  ObjString** keys = makeKeyArray("lookupKey", count);

  for (int i = 0; i < count; i++) {
    tableSet(&table, keys[i], NUMBER_VAL(i));
  }

  Value value;
  int hits = 0;
  double start = nowSeconds();

  for (int r = 0; r < rounds; r++) {
    for (int i = 0; i < count; i++) {
      if (tableGet(&table, keys[i], &value)) {
        hits++;
      }
    }
  }

  double end = nowSeconds();

  printf("lookup %d keys x %d rounds: %.6f sec, number of hits: %d\n",
       count, rounds, end - start, hits);

  free(keys);
  freeTable(&table);
}

static void benchDeleteReinsert(int count) {
  Table table;
  initTable(&table);

  ObjString** originalKeys = makeKeyArray("deleteKey", count);
  ObjString** newKeys = makeKeyArray("newKey", count / 2);

  for (int i = 0; i < count; i++) {
    tableSet(&table, originalKeys[i], NUMBER_VAL(i));
  }

  double start = nowSeconds();

  for (int i = 0; i < count; i += 2) {
    tableDelete(&table, originalKeys[i]);
  }

  for (int i = 0; i < count / 2; i++) {
    tableSet(&table, newKeys[i], NUMBER_VAL(count + i));
  }

  double end = nowSeconds();

  printf("delete half + reinsert half (%d total start): %.6f sec\n",
         count, end - start);

  free(originalKeys);
  free(newKeys);
  freeTable(&table);
}

int main(int argc, const char* argv[]) {
  (void)argc;
  (void)argv;

  initVM();

  benchInsert(50000);
  benchLookup(10000, 200);
  benchDeleteReinsert(50000);

  freeVM();
  return 0;
}