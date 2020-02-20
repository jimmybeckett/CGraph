#include <stdlib.h>
#include <check.h>
#include <stdio.h>
#include <HashTable.h>

HashTable *hash_table;

void setup() {
  hash_table = HashTable_Allocate();
}

void doNothing(void *unused) {}

void teardown() {
  HashTable_Free(hash_table, doNothing);
}

START_TEST(size) {
  ck_assert(true);
}
END_TEST

Suite *basic_suite(void) {
  Suite *suite = suite_create("HashTable Basic Test Suite");

  TCase *tc_basic = tcase_create("Basic Operations");
  tcase_add_checked_fixture(tc_basic, setup, teardown);

  tcase_add_test(tc_basic, size);

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
