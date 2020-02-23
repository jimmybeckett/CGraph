#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <HashTable.h>

// FNV-1a hash implementation
uint64_t hashString(char *str);

uint64_t hashInt(uint64_t n);

void freeKv(KeyValue_t kv);

bool compareInt(uint64_t int1, uint64_t int2);

int main(int argc, char** argv) {
  srand(420);

  HashTable *table = HashTable_Allocate((HT_HashFunction_t *) &hashString, (HT_KeyComparator_t *) &strcmp);

  // initialize strings so that allocation time doesn't screw with with timing
  const size_t num_strings = 20000;
  char **ref_array = (char**) malloc(num_strings * sizeof(char*));  // used to compare with hashtable
  const size_t string_len = 8;
  char **strings = (char**) malloc(num_strings * sizeof(char*));
  for (size_t i = 0; i < num_strings; i++) {
    strings[i] = (char*) malloc((string_len + 1) * sizeof(char*));
    for (int j = 0; j < string_len; j++) {
      strings[i][j] = (rand() % 26) + 97;  // random lowercase letter
    }
    strings[i][string_len] = '\0';
    ref_array[i] = strings[i];
  }

  // hashtable insert
  clock_t start = clock();
  for (size_t i = 0; i < num_strings; i++) {
    HashTable_Insert(table, (KeyValue_t) { .key = strings[i], .value = strings[i] });
  }
  clock_t end = clock();
  printf("time to execute %zu insert operations using a chained hash table: %f sec\n", num_strings, 
      (end - start) / (double) CLOCKS_PER_SEC);

  // array insert
  start = clock();
  for (size_t i = 0; i < num_strings; i++) {
    ref_array[i] = strings[i];
  }
  end = clock();
  printf("time to execute %zu insert operations using an array: %f sec\n", num_strings, 
      (end - start) / (double) CLOCKS_PER_SEC);

  // randomize string ordering
  for (int i = 0; i < num_strings; i++) {
    char *temp = ref_array[i];
    int r = rand() % num_strings;
    strings[i] = strings[r];
    strings[r] = temp;
  }

  // hashtable get (successful)
  start = clock();
  KeyValue_t kv;
  for (size_t i = 0; i < num_strings; i++) {
    HashTable_Get(table, strings[i], &kv);
  }
  end = clock();
  printf("time to execute %zu successful find operations using a chained hash table: %f sec\n", num_strings, 
      (end - start) / (double) CLOCKS_PER_SEC);

  // array get (successful)
  start = clock();
  for (size_t i = 0; i < num_strings; i++) {
    for (size_t j = 0; j < num_strings; j++) {
      if (strcmp(ref_array[j], strings[i]) == 0) {
        break;  // string found
      }
    }
  }
  end = clock();
  printf("time to execute %zu successful find operations using an array: %f sec\n", num_strings, 
      (end - start) / (double) CLOCKS_PER_SEC);

  // cleanup
  free(strings);  // actual strings are freed when freeing hashtable
  HashTable_Free(table, freeKv);

  printf("beginning hash table performance scaling benchmark\n");
  table = HashTable_Allocate((HT_HashFunction_t *) &hashInt, (HT_KeyComparator_t *) &compareInt);
  const size_t n = 10000000;
  const size_t m = 1000;
  start = clock();
  for (size_t i = 0; i < m; i++) {
    HashTable_Insert(table, (KeyValue_t) { (void *) i, (void *) i });
  }
  end = clock();
  printf("time for first %zu inserts: %f sec\n", m, (end - start) / (double) CLOCKS_PER_SEC);

  printf("executing %zu inserts\n", n);
  for (size_t i = 0; i < n; i++) {
    HashTable_Insert(table, (KeyValue_t) { (void *) i, (void *) i });
  }

  start = clock();
  for (size_t i = 0; i < m; i++) {
    HashTable_Insert(table, (KeyValue_t) { (void *) i, (void *) i });
  }
  end = clock();
  printf("time for last %zu inserts: %f sec\n", m, (end - start) / (double) CLOCKS_PER_SEC);

  HashTable_Free(table, NULL);

  return 0;
}

uint64_t hashString(char *str) {
  uint64_t hash = 14695981039346656037U;
  for (char *c = str; *c != '\0'; c++) {
    hash *= 1099511628211U;
    hash ^= *c;
  }
  return hash;
}

uint64_t hashInt(uint64_t n) {
  uint64_t hash = 14695981039346656037U;
  while (n != 0) {
    hash *= 1099511628211U;
    hash ^= n & 0XFF;
    n >>= 8;
  }
  return hash;
}

void freeKv(KeyValue_t kv) {
  free(kv.key);
  // free(kv.value);  in this case key == value
}

bool compareInt(uint64_t int1, uint64_t int2) {
  return int1 == int2;
}
