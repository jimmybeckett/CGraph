#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <HashTable.h>

/**************************************
 * Constants
**************************************/

#define INITIAL_CAPACITY (2 << 4)
#define LOAD_FACTOR .75

/**************************************
 * Struct definitions
**************************************/

struct hashtable_t {  // HashTable (declared in HashTable.h)
  // LinkedList **buckets;
  size_t size;
  size_t capacity;
  HT_KeyComparator_t *key_comparator;
  HT_HashFunction_t *hash_function;
};

/**************************************
 * Static helper function prototypes
**************************************/

// Returns true on success and false on failure
static bool ResizeIfNecessary(HashTable *table);

/**************************************
 * Functions in HashTable.h
**************************************/

HashTable *HashTable_Allocate(HT_KeyComparator_t *key_comparator, HT_HashFunction_t *hash_function) {
}

void HashTable_Free(HashTable *table, HT_FreeFunction_t *free_function) {
}

size_t HashTable_Size(HashTable *table) {
  return table->size;
}

bool HashTable_IsEmpty(HashTable *table) {
  return table->size == 0;
}

bool HashTable_Insert(HashTable *table, KeyValue_t keyValue) {
}

bool HashTable_Get(HashTable *table, Key_t key, KeyValue_t *output) {
}

bool HashTable_Remove(HashTable *table, Key_t key, KeyValue_t *output) {
}

/**************************************
 * Static helper functions
**************************************/
static bool ResizeIfNecessary(HashTable *table) {
}
