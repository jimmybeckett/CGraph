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

  LinkedList_PushHead(ll, malloc(sizeof(int)));
  ck_assert_int_eq(LinkedList_Size(ll), 1);

  LinkedList_PushTail(ll, malloc(sizeof(int)));
  ck_assert_int_eq(LinkedList_Size(ll), 2);

  LinkedList_PopHead(ll);
  ck_assert_int_eq(LinkedList_Size(ll), 1);

  LinkedList_PopTail(ll);
  ck_assert_int_eq(LinkedList_Size(ll), 0);
}
END_TEST

START_TEST(is_empty) {
  ck_assert(LinkedList_IsEmpty(ll));

  LinkedList_PushHead(ll, malloc(sizeof(int)));

  ck_assert(!LinkedList_IsEmpty(ll));

  LinkedList_PopTail(ll);

  ck_assert(LinkedList_IsEmpty(ll));
} 
END_TEST

START_TEST(push_pop_head) {
  int n = 10;
  for (int i = 1; i <= n; i++) {
    LinkedList_PushHead(ll, alloc_int(i));
  }
  for (int i = n; i > 0; i--) {
    int *j = LinkedList_PopHead(ll);
    ck_assert_int_eq(*j, i);
    free(j);
  }
}
END_TEST

START_TEST(push_pop_tail) {
  int n = 10;
  for (int i = 1; i <= n; i++) {
    LinkedList_PushTail(ll, alloc_int(i));
  }
  for (int i = n; i > 0; i--) {
    int *j = LinkedList_PopTail(ll);
    ck_assert_int_eq(*j, i);
    free(j);
  }
}
END_TEST

START_TEST(push_pop_head_tail) {
  LinkedList_PushHead(ll, alloc_int(2));
  LinkedList_PushHead(ll, alloc_int(1));
  LinkedList_PushTail(ll, alloc_int(3));
  // ll: <-> 1 <-> 2 <-> 3 <->
 
  int *n = LinkedList_PopHead(ll);
  ck_assert_int_eq(*n, 1);
  free(n);

  n = LinkedList_PopTail(ll);
  ck_assert_int_eq(*n, 3);
  free(n);

  n = LinkedList_PopHead(ll);
  ck_assert_int_eq(*n, 2);
  free(n);
}
END_TEST

Suite *basic_suite(void) {
  Suite *suite = suite_create("LinkedList Basic Suite");
  TCase *tc_core = tcase_create("Core");

  tcase_add_checked_fixture(tc_core, setup, teardown);

  tcase_add_test(tc_core, size);
  tcase_add_test(tc_core, is_empty);
  tcase_add_test(tc_core, push_pop_head);
  tcase_add_test(tc_core, push_pop_tail);
  tcase_add_test(tc_core, push_pop_head_tail);

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
