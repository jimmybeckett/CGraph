#include <stdlib.h>
#include <LinkedList.h>
#include <check.h>

LinkedList *ll;

void setup(void) {
    ll = LinkedList_Allocate();
}

void teardown(void) {
    LinkedList_Free(ll);
}

int *alloc_int(int val) {
  int *n = (int*) malloc(sizeof(int));
  *n = val;
  return n;
}

START_TEST(size) {
  ck_assert_int_eq(LinkedList_Size(ll), 0);

  LinkedList_Push(ll, malloc(sizeof(int)));
  ck_assert_int_eq(LinkedList_Size(ll), 1);

  LinkedList_Push(ll, malloc(sizeof(int)));
  ck_assert_int_eq(LinkedList_Size(ll), 2);

  LinkedList_Pop(ll);
  ck_assert_int_eq(LinkedList_Size(ll), 1);

  LinkedList_Pop(ll);
  ck_assert_int_eq(LinkedList_Size(ll), 0);

  LinkedList_Pop(ll);  // pop from empty list
  ck_assert_int_eq(LinkedList_Size(ll), 0);
}
END_TEST

START_TEST(is_empty) {
  ck_assert(LinkedList_IsEmpty(ll));

  LinkedList_Push(ll, malloc(sizeof(int)));

  ck_assert(!LinkedList_IsEmpty(ll));

  LinkedList_Pop(ll);

  ck_assert(LinkedList_IsEmpty(ll));
} 
END_TEST

START_TEST(push_pop) {
  int n = 10;
  for (int i = 1; i <= n; i++) {
    LinkedList_Push(ll, alloc_int(i));
  }
  for (int i = n; i > 0; i--) {
    int *j = LinkedList_Pop(ll);
    ck_assert_int_eq(*j, i);
    free(j);
  }
}
END_TEST

Suite *basic_suite(void) {
  Suite *suite = suite_create("LinkedList Basic Suite");
  TCase *tc_core = tcase_create("Core");

  tcase_add_checked_fixture(tc_core, setup, teardown);

  tcase_add_test(tc_core, size);
  tcase_add_test(tc_core, is_empty);
  tcase_add_test(tc_core, push_pop);

  suite_add_tcase(suite, tc_core);
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
