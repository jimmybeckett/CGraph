#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <HashTable.h>
#include <LinkedList.h>

/**************************************
 * Constants
**************************************/

#define INITIAL_CAPACITY (2 << 4)
#define LOAD_FACTOR .75

/**************************************
 * Struct definitions
**************************************/

struct hashtable_t {  // HashTable (declared in HashTable.h)
  LinkedList **buckets;
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
  HashTable *table = (HashTable *) malloc(sizeof(HashTable));
  if (table == NULL) return NULL;
  *table = (HashTable) { .buckets = (LinkedList **) malloc(INITIAL_CAPACITY * sizeof(LinkedList *)),
                          .size = 0, 
                          .capacity = INITIAL_CAPACITY,
                          .key_comparator = key_comparator,
                          .hash_function = hash_function };
  if (table->buckets == NULL) return NULL;

  // fill hash_table->buckets with empty buckets
  for (size_t i = 0; i < table->capacity; i++) {
    table->buckets[i] = LinkedList_Allocate();
    if (table->buckets[i] == NULL) {  // check for LL_Alloc error
      // free already allocated buckets
      for (size_t j = 0; j < i; j++) {
        LinkedList_Free(table->buckets[j], NULL);  // table->buckets[j] is empty
      }
      free(table->buckets);
      free(table);
      return NULL;
    }
  }

  return table;
}

void HashTable_Free(HashTable *table, HT_FreeFunction_t *free_function) {
  for (size_t i = 0; i < table->capacity; i++) {
    LinkedList_Free(table->buckets[i], (LL_FreeFunction_t *) free_function);
  }
  free(table->buckets);
  free(table);
}

size_t HashTable_Size(HashTable *table) {
  return table->size;
}

bool HashTable_IsEmpty(HashTable *table) {
  return table->size == 0;
}

bool HashTable_Insert(HashTable *table, KeyValue_t keyValue) {
  LinkedList *bucket = table->buckets[table->hash_function(keyValue.key) % table->capacity];
  KeyValue_t *old_keyValue;
  if (LinkedList_Find(bucket, &keyValue, (void **) &old_keyValue, (LL_Comparator_t *) table->key_comparator)) {
    // the keys may be "equal" under the comparator, but still be different
    old_keyValue->key = keyValue.key;  
    old_keyValue->value = keyValue.value;
    return true;
  }

  // key does not exist in table, make and insert a new keyvalue
  KeyValue_t *keyValue_cpy = (KeyValue_t *) malloc(sizeof(KeyValue_t));
  if (keyValue_cpy == NULL) return false;
  *keyValue_cpy = (KeyValue_t) { .key = keyValue.key, .value = keyValue.value };

  if (!LinkedList_PushHead(bucket, keyValue_cpy)) {
    free(keyValue_cpy);
    return false;
  }

  table->size++;

  if (!ResizeIfNecessary(table)) {
    KeyValue_t *unused;
    LinkedList_RemoveIfPresent(bucket, keyValue_cpy, (void **) &unused, (LL_Comparator_t *) table->key_comparator);
    return false;
  }

  return true;
}

bool HashTable_Get(HashTable *table, Key_t key, Value_t *output) {
  LinkedList *bucket = table->buckets[table->hash_function(key) % table->capacity];

  KeyValue_t keyValue = { .key = key, .value = NULL };
  KeyValue_t *result;
  if (LinkedList_Find(bucket, &keyValue, (void **) &result, (LL_Comparator_t *) table->key_comparator)) {
    *output = result->value;
    return true;
  }
  return false;
}

// Value_t HashTable_Remove(HashTable *table, Key_t key);

/**************************************
 * Static helper functions
**************************************/
static bool ResizeIfNecessary(HashTable *table) {
  if (table->size <= table->capacity * LOAD_FACTOR) {  // check if resize needed
    return true;
  }

  // double the capacity
  size_t new_capacity = 2 * table->capacity;
  LinkedList** new_buckets = (LinkedList **) malloc(new_capacity * sizeof(LinkedList *));
  if (new_buckets == NULL) return 0;

  // fill new_buckets with empty buckets
  for (size_t i = 0; i < new_capacity; i++) {
    new_buckets[i] = LinkedList_Allocate();
    if (new_buckets[i] == NULL) {  // check for LL_Alloc failure
      // cleanup
      for (size_t j = 0; j < i; j++) {
        LinkedList_Free(new_buckets[i], NULL);  // new_buckets[i] is empty
      }
      free(new_buckets);
      return false;
    }
  }

  // transfer from table->buckets to new_buckets
  for (size_t i = 0; i < table->capacity; i++) {
    LLIterator *iter = LLIterator_Allocate(table->buckets[i]);
    for (KeyValue_t *kv; LLIterator_Get(iter, (void **) &kv); LLIterator_Next(iter)) {
      if (!LinkedList_PushHead(new_buckets[table->hash_function(kv->key) % new_capacity], kv)) {
        // clean up
        for (size_t j = 0; j < new_capacity; j++) {
          LinkedList_Free(new_buckets[j], NULL);  // doesn't free the actual elements
        }
        free(new_buckets);
        return false;
      }
    }
  }

  // free table->buckets
  for (size_t i = 0; i < table->capacity; i++) {
    LinkedList_Free(table->buckets[i], NULL);  // doesn't free the actual elements
  }
  free(table->buckets);

  table->buckets = new_buckets;
  table->capacity = new_capacity;

  return true;
}
