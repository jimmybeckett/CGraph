#include <stdlib.h> 
#include <check.h>
#include <stdio.h>
#include <HashTable.h>

HashTable *table;

bool keyComp(KeyValue_t *kv1, KeyValue_t *kv2) {
  return (int64_t) kv1->key == (int64_t) kv2->key;
}

// pretends v is an int
Hash_t hash(Value_t v) {
  return ((Hash_t) v) % 10;
}

void setup() {
  table = HashTable_Allocate(keyComp, hash);
}

void teardown() {
  HashTable_Free(table, NULL);
}

START_TEST(size) {
  ck_assert_int_eq(HashTable_Size(table), 0);
  
  HashTable_Insert(table, (KeyValue_t) { .key = (void *) 1, .value = NULL });

  ck_assert_int_eq(HashTable_Size(table), 1);

  // insert duplicate key and hash
  HashTable_Insert(table, (KeyValue_t) { .key = (void *) 1, .value = NULL });

  ck_assert_int_eq(HashTable_Size(table), 1);

  // insert duplicate hash only (hash is key % 10)
  HashTable_Insert(table, (KeyValue_t) { .key = (void *) 11, .value = NULL });
}
END_TEST

START_TEST(insert_get) {
  // HT: []
  Value_t v;
  ck_assert(!HashTable_Get(table, (void *) 1, &v));

  HashTable_Insert(table, (KeyValue_t) { .key = (void *) 1, .value = (void *) 2 });
  // HT: [1 : {1, 2}]

  ck_assert(HashTable_Get(table, (void *) 1, &v));
  ck_assert_int_eq((int) v, 2);

  ck_assert(!HashTable_Get(table, (void *) 2, &v));
  ck_assert(!HashTable_Get(table, (void *) 11, &v));  // hash(11) == 11 % 10 == 1

  HashTable_Insert(table, (KeyValue_t) { .key = (void *) 11, .value = (void *) 3 });
  // HT: [1 : {1, 2}, {11, 3}]

  ck_assert(HashTable_Get(table, (void *) 1, (void **) &v));
  ck_assert_int_eq((int) v, 2);

  ck_assert(HashTable_Get(table, (void *) 11, (void **) &v));
  ck_assert_int_eq((int) v, 3);
}
END_TEST

START_TEST(resize) {
  // a nice random-ish sequence
  for (int64_t i = 0; i < 10000; i += 7 + i % 13) {
    HashTable_Insert(table, (KeyValue_t) { .key = (void *) i, .value = (void *) (i + 1) });
  }

  Value_t v;
  for (int64_t i = 0; i < 10000; i += 7 + i % 13) {
    ck_assert(HashTable_Get(table, (void *) i, &v));
    ck_assert_int_eq((int) v, i + 1);

    ck_assert(!HashTable_Get(table, (void *) (i + 1), &v));
  }
}
END_TEST

Suite *basic_suite(void) {
  Suite *suite = suite_create("HashTable Basic Test Suite");

  TCase *tc_basic = tcase_create("Basic Operations");
  tcase_add_checked_fixture(tc_basic, setup, teardown);

  tcase_add_test(tc_basic, size);
  tcase_add_test(tc_basic, insert_get);
  tcase_add_test(tc_basic, resize);

  suite_add_tcase(suite, tc_basic);
  return suite;
}

int main() {
  Suite *s = basic_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
