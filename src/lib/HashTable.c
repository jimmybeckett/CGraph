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

// Returns 0 on failure, and the new capacity of the table otherwise
// static size_t ResizeIfNecessary(HashTable *table);

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
        LinkedList_Free(table->buckets[j], NULL);  // buckets are empty, so NULL free function is fine
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

  /*
  if (ResizeIfNecessary(table) == 0) {
    // TODO: remove newly inserted keyvalue from bucket
    return false;
  } */

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
/*
static size_t ResizeIfNecessary(HashTable *table) {
  if (table->size <= table->capacity * LOAD_FACTOR) {  // check if resize needed
    return table->capacity;
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
        // new_buckets[i] is empty, so the free function (NULL) is never called
        LinkedList_Free(new_buckets[i], NULL); 
      }
      free(new_buckets);
      return 0;
    }
  }

  for (size_t i = 0; i < table->capacity; i++) {
    LinkedList *bucket = table->buckets[i];
    // TODO: use an iterator
    for (size_t j = 0; j < LinkedList_Size(bucket); j++) {
      KeyValue_t *kv = LinkedList_PopHead(bucket);

      // TODO: this is stupidly inefficient
      if (PutIntoBucket(new_buckets[kv->key % new_capacity], kv->key, kv->value) == 0) {
        // TODO: clean up everything
        return 0;
      }
      free(kv);
    }

    // bucket is empty, so the free function (NULL) is never called
    LinkedList_Free(bucket, NULL);  
  }

  free(table->buckets);
  table->buckets = new_buckets;
  table->capacity = new_capacity;
  return table->capacity;
} */
