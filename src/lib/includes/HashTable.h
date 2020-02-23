#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct hashtable HashTable;

typedef uint64_t Hash_t;

typedef void *Key_t;

typedef void *Value_t;

typedef struct keyvalue_t {
  Key_t key;
  Value_t value;
} KeyValue_t;

typedef void (HT_FreeFunction_t(KeyValue_t keyValue));

typedef Hash_t (HT_HashFunction_t(Key_t key));

typedef bool (HT_KeyComparator_t(Key_t key1, Key_t key2));

HashTable *HashTable_Allocate(HT_HashFunction_t *hash_function, HT_KeyComparator_t *key_comparator);

void HashTable_Free(HashTable *table, HT_FreeFunction_t *free_function);

size_t HashTable_Size(HashTable *table);

bool HashTable_IsEmpty(HashTable *table);

bool HashTable_Insert(HashTable *table, KeyValue_t kv);

bool HashTable_Get(HashTable *table, Key_t key, KeyValue_t *found_kv);

bool HashTable_Remove(HashTable *table, Key_t key, KeyValue_t *removed_kv);

#endif  // HASH_TABLE_H_
