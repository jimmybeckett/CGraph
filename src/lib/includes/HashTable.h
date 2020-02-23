#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Represents a chained hash table.
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

// Allocates a new HashTable.
// Returns NULL on failure and true on success.
HashTable *HashTable_Allocate(HT_HashFunction_t *hash_function, HT_KeyComparator_t *key_comparator);

// Deallocates the hashtable, calls free_function on each stored keyValue if free_function != NULL.
void HashTable_Free(HashTable *table, HT_FreeFunction_t *free_function);

// Returns the number of elements currently in the hashtable.
size_t HashTable_Size(HashTable *table);

// Returns true if the hashtable is empty, and 0 otherwise.
bool HashTable_IsEmpty(HashTable *table);

// Inserts kv into the hashtable.
// Responsiblity for deallocating the key and value held by kv is transferred to the hashtable.
// Returns false on failure and true on success.
bool HashTable_Insert(HashTable *table, KeyValue_t kv);

// Uses the table's hashFunction and keyComparator to find and return a keyValue matching key through found_kv.
// The hashtable retains responsiblity for deallocating the returned keyValue.
// Returns true if a matching keyValue is found and false otherwise.
bool HashTable_Get(HashTable *table, Key_t key, KeyValue_t *found_kv);

// Uses the table's hashFunction and keyComparator to find, remove, and return a keyValue matching key through found_kv.
// Responsiblity for deallocating the returned keyValue is passed to the caller.
// Returns true if a matching keyValue is found and false otherwise.
bool HashTable_Remove(HashTable *table, Key_t key, KeyValue_t *removed_kv);

#endif  // HASH_TABLE_H_
