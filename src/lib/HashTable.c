#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <HashTable.h>

/**************************************
 * Constants
**************************************/

#define INITIAL_CAPACITY (2 << 4)
#define LOAD_FACTOR .75

/**************************************
 * Struct definitions
**************************************/

typedef struct node {
  KeyValue_t kv;
  struct node *next;
} Node;

struct hashtable_t {  // HashTable (declared in HashTable.h)
  Node **buckets;
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

// Frees every node in bucket, and calls free_function on each node's kv
static void FreeBucket(Node *bucket, HT_FreeFunction_t *free_function);

// Returns a pointer to the bucket corresponding to key
static Node **GetBucket(HashTable *table, Key_t key);

/**************************************
 * Functions in HashTable.h
**************************************/

HashTable *HashTable_Allocate(HT_HashFunction_t *hash_function, HT_KeyComparator_t *key_comparator) {
  HashTable *table = (HashTable*) malloc(sizeof(HashTable));
  if (table == NULL) return NULL;
  *table = (HashTable) { .buckets = (Node**) calloc(INITIAL_CAPACITY, sizeof(Node*)),
                         .size = 0,
                         .capacity = INITIAL_CAPACITY,
                         .hash_function = hash_function,
                         .key_comparator = key_comparator };
  if (table->buckets == NULL) {
    free(table);
    return NULL;
  }
  return table;
}

void HashTable_Free(HashTable *table, HT_FreeFunction_t *free_function) {
  for (size_t i = 0; i < table->capacity; i++) {
    FreeBucket(table->buckets[i], free_function);
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

bool HashTable_Insert(HashTable *table, KeyValue_t kv) {
  Node **bucket = GetBucket(table, kv.key);

  // look through the bucket for key that's "equal" to kv->key
  Node *curr = *bucket;
  while (curr != NULL) {
    if (table->key_comparator(curr->kv.key, kv.key)) {
      // must update key (in addition to value) bc key_comparator could return true even if the keys are not identical
      curr->kv.key = kv.key;
      curr->kv.value = kv.value;
      return true;  // we don't update size bc we replaced instead of added
    }
    curr = curr->next;
  }

  // either the bucket is empty or the key wasn't found in the bucket
  Node *new_node = (Node*) malloc(sizeof(Node));
  if (new_node == NULL) return false;
  *new_node = (Node) { .kv = kv, .next = *bucket /* aka NULL if the bucket is empty */ };
  *bucket = new_node;

  table->size++;
  return true;
}

bool HashTable_Get(HashTable *table, Key_t key, KeyValue_t *output) {
  Node **bucket = GetBucket(table, key);
  
  Node *curr = *bucket;
  while (curr != NULL) {
    if (table->key_comparator(curr->kv.key, key)) {
      *output = curr->kv;
      return true;
    }
    curr = curr->next;
  }

  return false;
}

bool HashTable_Remove(HashTable *table, Key_t key, KeyValue_t *output) {
}

/**************************************
 * Static helper functions
**************************************/
static bool ResizeIfNecessary(HashTable *table) {
}

static void FreeBucket(Node *bucket, HT_FreeFunction_t *free_function) {
  Node *curr = bucket;
  while (curr != NULL) {
    Node *old_curr = curr;
    curr = curr->next;
    if (free_function != NULL) {
      free_function(old_curr->kv);
    }
    free(old_curr);
  }
}

static Node **GetBucket(HashTable *table, Key_t key) {
  return &table->buckets[table->hash_function(key) % table->capacity];
}
