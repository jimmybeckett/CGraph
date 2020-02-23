#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <HashTable.h>

/**************************************
 * Constants
**************************************/

#define INITIAL_CAPACITY (2 << 4)
#define CAPACITY_INC_MULTIPLIER 2  // capacity *= CAPACITY_INC_MULTIPLIER when increasing capacity
#define LOAD_FACTOR .75  // increase capacity when size > capacity * LOAD_FACTOR

/**************************************
 * Struct definitions
**************************************/

typedef struct node {
  KeyValue_t kv;
  struct node *next;
} Node;

struct hashtable {  // HashTable (declared in HashTable.h)
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
    // free each bucket
    Node *curr = table->buckets[i];
    while (curr != NULL) {
      Node *old_curr = curr;
      curr = curr->next;
      if (free_function != NULL) {
        free_function(old_curr->kv);
      }
      free(old_curr);
    }
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
  if (!ResizeIfNecessary(table)) return false;

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

bool HashTable_Get(HashTable *table, Key_t key, KeyValue_t *found_kv) {
  Node **bucket = GetBucket(table, key);
  
  Node *curr = *bucket;
  while (curr != NULL) {
    if (table->key_comparator(curr->kv.key, key)) {
      *found_kv = curr->kv;
      return true;
    }
    curr = curr->next;
  }

  return false;
}

bool HashTable_Remove(HashTable *table, Key_t key, KeyValue_t *removed_kv) {
  Node **bucket = GetBucket(table, key);

  if (*bucket == NULL) return false;

  if (table->key_comparator((*bucket)->kv.key, key)) {  // check if target key is the first node in the bucket
    Node *head = *bucket;
    *bucket = (*bucket)->next;
    *removed_kv = head->kv;
    free(head);
    return true;
  }

  // look through the appropriate bucket for the target key
  Node *curr = *bucket;
  while (curr->next != NULL) {
    if (table->key_comparator(curr->next->kv.key, key)) {
      Node *old_next = curr->next;
      curr->next = curr->next->next;
      *removed_kv = old_next->kv;
      free(old_next);
      return true;
    }
    curr = curr->next;
  }

  return false;
}

/**************************************
 * Static helper functions
**************************************/
static bool ResizeIfNecessary(HashTable *table) {
  if (table->capacity * LOAD_FACTOR > table->size) return true;  // resize not necessary :)

  size_t new_capacity = CAPACITY_INC_MULTIPLIER * table->capacity;
  Node **new_buckets = (Node**) calloc(new_capacity, sizeof(Node*));
  if (new_buckets == NULL) return false;

  size_t old_capacity = table->capacity;
  table->capacity = new_capacity;
  Node **old_buckets = table->buckets;
  table->buckets = new_buckets;

  // move all nodes from old_buckets to table->buckets (aka new_buckets)
  for (size_t i = 0; i < old_capacity; i++) {
    Node *curr = old_buckets[i];
    while (curr != NULL) {
      Node *old_curr = curr;
      curr = curr->next;
      Node** new_bucket = GetBucket(table, old_curr->kv.key);
      old_curr->next = *new_bucket;
      *new_bucket = old_curr;
    }
  }

  free(old_buckets);
  return true;
}

static Node **GetBucket(HashTable *table, Key_t key) {
  return &table->buckets[table->hash_function(key) % table->capacity];
}
