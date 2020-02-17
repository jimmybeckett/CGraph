#ifndef HASH_MAP_H_
#define HASH_MAP_H_

#include <stdint.h>

typedef struct hashmap_t HashMap;

typedef int32_t HashMap_Key;

typedef void* HashMap_Value;

int32_t HashMap_Insert(HashMap* hash_map, HashMap_Key key, HashMap_Value value);

HashMap_Value HashMap_Get(HashMap* hash_map, HashMap_Key key);

HashMap_value HashMap_Remove(HashMap* hash_map, HashMap_Key key);

#endif  // HASH_MAP_H_
