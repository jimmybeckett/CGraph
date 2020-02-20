#include <stdbool.h>
#include <stdlib.h>
#include <HashTable.h>
#include <LinkedList.h>

#define INITIAL_CAPACITY (2 >> 4)
#define LOAD_FACTOR .75

typedef struct keyvalue_t {
  Key_t key;
  Value_t value;
} KeyValue_t;

struct hashtable_t {  // HashMap
  LinkedList **buckets;
  int64_t size;
  int64_t capacity;
};

HashTable *HashTable_Allocate() {
  HashTable *table = (HashTable *) malloc(sizeof(HashTable));
  if (table == NULL)  // check for malloc failure
    return NULL;
  *table = (HashTable) { .buckets = (LinkedList **) malloc(INITIAL_CAPACITY * sizeof(LinkedList *)),
                          .size = 0, 
                          .capacity = INITIAL_CAPACITY };
  // fill hash_table->buckets with empty buckets
  for (int64_t i = 0; i < table->capacity; i++) {
    table->buckets[i] = LinkedList_Allocate();
  }
  return table;
}

void HashTable_Free(HashTable *table, void (free_function(Value_t value))) {
  for (int64_t i = 0; i < table->capacity; i++) {
    LinkedList_Free(table->buckets[i], free_function);
  }
  free(table->buckets);
  free(table);
}

// bool HashTable_Insert(HashTable *table, Key_t key, Value_t value);

// Value_t HashTable_Get(HashMap *table, Key_t key);

// Value_t HashTable_Remove(HashTable *table, Key_t key);
