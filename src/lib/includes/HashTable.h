#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct hashtable_t HashTable;

typedef int64_t Key_t;

typedef void *Value_t;

HashTable *HashTable_Allocate();

void HashTable_Free(HashTable *table, void (free_function(Value_t value)));

// bool HashTable_Insert(HashTable *table, Key_t key, Value_t value);

// Value_t HashTable_Get(HashMap *table, Key_t key);

// Value_t HashTable_Remove(HashMap *table, Key_t key);

#endif  // HASH_TABLE_H_
