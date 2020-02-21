#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct hashtable_t HashTable;

typedef uint64_t Hash_t;

typedef void *Key_t;

typedef void *Value_t;

typedef struct KeyValue_t {
  Key_t key;
  Value_t value;
} KeyValue_t;

typedef void (HT_FreeFunction_t(KeyValue_t *keyValue));

typedef Hash_t (HT_HashFunction_t(Key_t key));

typedef bool (HT_KeyComparator_t(KeyValue_t *kv1, KeyValue_t *kv2));

HashTable *HashTable_Allocate(HT_KeyComparator_t *key_comparator, HT_HashFunction_t *hash_function);

void HashTable_Free(HashTable *table, HT_FreeFunction_t *free_function);

size_t HashTable_Size(HashTable *table);

bool HashTable_Insert(HashTable *table, KeyValue_t keyValue);

bool HashTable_Get(HashTable *table, Key_t key, Value_t *result);

// Value_t HashTable_Remove(HashMap *table, Key_t key);

#endif  // HASH_TABLE_H_
