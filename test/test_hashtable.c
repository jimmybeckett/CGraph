#include <stdlib.h> 
#include <check.h>
#include <stdio.h>
#include <HashTable.h>

HashTable *table;

// pretends v is an int64_t
Hash_t HashInt(Value_t v) {
  return ((Hash_t) v) % 10;
}

// pretends keys are int64_t's
bool IntKeyComp(Key_t key1, Key_t key2) {
  return (int64_t) key1 == (int64_t) key2;
}

void setup() {
  table = HashTable_Allocate(HashInt, IntKeyComp);
  ck_assert(table != NULL);
}

void teardown() {
  HashTable_Free(table, NULL);
}

START_TEST(size) {
  ck_assert_int_eq(HashTable_Size(table), 0);

  ck_assert(HashTable_Insert(table, (KeyValue_t) { .key = (void *) 1, .value = NULL }));

  ck_assert_int_eq(HashTable_Size(table), 1);

  // insert duplicate key and hash
  ck_assert(HashTable_Insert(table, (KeyValue_t) { .key = (void *) 1, .value = NULL }));

  ck_assert_int_eq(HashTable_Size(table), 1);

  // insert duplicate hash only (hash is key % 10)
  ck_assert(HashTable_Insert(table, (KeyValue_t) { .key = (void *) 1, .value = NULL }));
}
END_TEST

START_TEST(insert_get) { 
  // HT: []
  KeyValue_t kv;
  ck_assert(!HashTable_Get(table, (void *) 1, &kv));

  ck_assert(HashTable_Insert(table, (KeyValue_t) { .key = (void *) 1, .value = (void *) 2 }));
  // HT: [1 : {1, 2}]

  ck_assert(HashTable_Get(table, (void *) 1, &kv));
  ck_assert_int_eq((int) kv.key, 1);
  ck_assert_int_eq((int) kv.value, 2);

  ck_assert(!HashTable_Get(table, (void *) 2, &kv));
  ck_assert(!HashTable_Get(table, (void *) 11, &kv));  // hash(11) == 11 % 10 == 1

  ck_assert(HashTable_Insert(table, (KeyValue_t) { .key = (void *) 11, .value = (void *) 3 }));
  // HT: [1 : {1, 2}, {11, 3}]

  ck_assert(HashTable_Get(table, (void *) 1, &kv));
  ck_assert_int_eq((int) kv.key, 1);
  ck_assert_int_eq((int) kv.value, 2);

  ck_assert(HashTable_Get(table, (void *) 11, &kv));
  ck_assert_int_eq((int) kv.key, 11);
  ck_assert_int_eq((int) kv.value, 3);
}
END_TEST

START_TEST(resize) {  // puts enough elements in the table to force a resize
  int n = 10000;
  // a nice random-ish sequence
  for (int64_t i = 0; i < n; i += 7 + i % 13) {
    ck_assert(HashTable_Insert(table, (KeyValue_t) { .key = (void *) i, .value = (void *) (i + 1) }));
  }

  KeyValue_t kv;
  for (int64_t i = 0; i < n; i += 7 + i % 13) {
    ck_assert(HashTable_Get(table, (void *) i, &kv));
    ck_assert_int_eq((int) kv.value, i + 1);

    ck_assert(!HashTable_Get(table, (void *) (i + 1), &kv));
  }
}
END_TEST


START_TEST(remove_test) {  // "remove" is already defined
  int n = 10;
  // a nice random-ish sequence
  for (int64_t i = 0; i < n; i++) {
    ck_assert(HashTable_Insert(table, (KeyValue_t) { .key = (void *) i, .value = (void *) (i + 1) }));
  }

  KeyValue_t kv;
  for (int64_t i = 0; i < n; i += 2) {
    ck_assert(HashTable_Remove(table, (void *) i, &kv));
    ck_assert_int_eq((int) kv.value, i + 1);

    ck_assert(!HashTable_Get(table, (void *) (i), &kv));  // check i was actually removed
    ck_assert(HashTable_Get(table, (void *) (i + 1), &kv));
    ck_assert_int_eq((int) kv.value, i + 2);
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
  tcase_add_test(tc_basic, remove_test);

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
