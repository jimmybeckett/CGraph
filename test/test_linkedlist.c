#include <stdlib.h>
#include <LinkedList.h>
#include <check.h>
#include <stdio.h>

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

  LinkedList_PopHead(ll);
  ck_assert_int_eq(LinkedList_Size(ll), 0);

  LinkedList_PopHead(ll);  // remove from empty list
  ck_assert_int_eq(LinkedList_Size(ll), 0);
}
END_TEST

START_TEST(is_empty) {
  ck_assert(LinkedList_IsEmpty(ll));

  LinkedList_PushHead(ll, malloc(sizeof(int)));

  ck_assert(!LinkedList_IsEmpty(ll));

  LinkedList_PopHead(ll);

  ck_assert(LinkedList_IsEmpty(ll));
} 
END_TEST

START_TEST(pop_empty) {
  void *ptr = LinkedList_PopHead(ll);
  ck_assert(ptr == NULL);
  ck_assert_int_eq(LinkedList_Size(ll), 0);
}
END_TEST

START_TEST(add_remove) {
  int n = 10;
  for (int i = n; i > 0; i--) {
    LinkedList_PushHead(ll, alloc_int(i));
  }
  // 1 -> 2 -> ... -> 10

  for (int i = n + 1; i <= 2*n; i++) {
    LinkedList_PushTail(ll, alloc_int(i));
  }
  // 1 -> 2 -> ... -> 10 -> 11 -> 12 -> ... -> 20

  for (int i = 1; i <= 2*n; i++) {
    int *j = LinkedList_PopHead(ll);
    ck_assert_int_eq(*j, i);
    free(j);
  }
}
END_TEST

START_TEST(peek_head) {
  ck_assert(LinkedList_PeekHead(ll) == NULL);

  LinkedList_PushHead(ll, alloc_int(1));
  LinkedList_PushHead(ll, alloc_int(2));

  int *n = LinkedList_PeekHead(ll);
  ck_assert_int_eq(*n, 2);

  LinkedList_PopHead(ll);

  n = LinkedList_PeekHead(ll);
  ck_assert_int_eq(*n, 1);
}
END_TEST

START_TEST(peek_tail) {
  ck_assert(LinkedList_PeekTail(ll) == NULL);

  LinkedList_PushTail(ll, alloc_int(1));
  LinkedList_PushTail(ll, alloc_int(2));

  int *n = LinkedList_PeekTail(ll);
  ck_assert_int_eq(*n, 2);

  LinkedList_PopHead(ll);

  n = LinkedList_PeekTail(ll);
  ck_assert_int_eq(*n, 2);
}
END_TEST

Suite *basic_suite(void) {
  Suite *suite = suite_create("LinkedList Basic Suite");

  TCase *tc_basic = tcase_create("Basic Operations");
  tcase_add_checked_fixture(tc_basic, setup, teardown);

  tcase_add_test(tc_basic, size);
  tcase_add_test(tc_basic, is_empty);
  tcase_add_test(tc_basic, pop_empty);
  tcase_add_test(tc_basic, add_remove);
  tcase_add_test(tc_basic, peek_head);
  tcase_add_test(tc_basic, peek_tail);

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
