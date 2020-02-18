#include <stdlib.h>
#include <LinkedList.h>
#include <check.h>
#include <stdio.h>

LinkedList *ll;
LLIterator *ll_iter;

void setup(void) {
  ll = LinkedList_Allocate();
  ll_iter = NULL;  // iterator can only be allocated after list is fully initialized
}

void teardown(void) {
  LinkedList_Free(ll);
  LLIterator_Free(ll_iter);
}

int *alloc_int(int val) {
  int *n = (int*) malloc(sizeof(int));
  *n = val;
  return n;
}

START_TEST(size) {
  ck_assert_int_eq(LinkedList_Size(ll), 0);

  LinkedList_Add(ll, malloc(sizeof(int)));
  ck_assert_int_eq(LinkedList_Size(ll), 1);

  LinkedList_Add(ll, malloc(sizeof(int)));
  ck_assert_int_eq(LinkedList_Size(ll), 2);

  LinkedList_Remove(ll);
  ck_assert_int_eq(LinkedList_Size(ll), 1);

  LinkedList_Remove(ll);
  ck_assert_int_eq(LinkedList_Size(ll), 0);

  LinkedList_Remove(ll);  // remove from empty list
  ck_assert_int_eq(LinkedList_Size(ll), 0);
}
END_TEST

START_TEST(is_empty) {
  ck_assert(LinkedList_IsEmpty(ll));

  LinkedList_Add(ll, malloc(sizeof(int)));

  ck_assert(!LinkedList_IsEmpty(ll));

  LinkedList_Remove(ll);

  ck_assert(LinkedList_IsEmpty(ll));
} 
END_TEST

START_TEST(remove_empty) {
  void *ptr = LinkedList_Remove(ll);
  ck_assert(ptr == NULL);
  ck_assert_int_eq(LinkedList_Size(ll), 0);
}
END_TEST

START_TEST(add_remove) {
  int n = 10;
  for (int i = 1; i <= n; i++) {
    LinkedList_Add(ll, alloc_int(i));
  }

  for (int i = n; i > 0; i--) {
    int *j = LinkedList_Remove(ll);
    ck_assert_int_eq(*j, i);
    free(j);
  }
}
END_TEST

START_TEST(head) {
  ck_assert(LinkedList_Head(ll) == NULL);

  LinkedList_Add(ll, alloc_int(1));
  LinkedList_Add(ll, alloc_int(2));

  int *n = LinkedList_Head(ll);
  ck_assert_int_eq(*n, 2);

  LinkedList_Remove(ll);

  n = LinkedList_Head(ll);
  ck_assert_int_eq(*n, 1);
}
END_TEST

START_TEST(iterator_get) {
  int m = 2, n = 10;
  for (int i = m; i < m + n; i++) {
    LinkedList_Add(ll, alloc_int(i));
  }

  ll_iter = LLIterator_Allocate(ll);

  int i = m + n - 1;
  while (LLIterator_IsValid(ll_iter)) {
    int *m = LLIterator_Get(ll_iter);
    ck_assert_int_eq(*m, i);
    LLIterator_Next(ll_iter);
    i--;
  }

  ck_assert_int_eq(i, m - 1);
  ck_assert(!LLIterator_IsValid(ll_iter));
}
END_TEST

START_TEST(iterator_remove) {
  for (int i = 4; i > 0; i--) {
    LinkedList_Add(ll, alloc_int(i));
  }
  // 1 -> 2 -> 3 -> 4 ->
  // |

  ll_iter = LLIterator_Allocate(ll);

  int *n = LLIterator_Remove(ll_iter);
  ck_assert_int_eq(*n, 1);

  // 2 -> 3 -> 4 ->
  // |

  n = LLIterator_Get(ll_iter);
  ck_assert_int_eq(*n, 2);
  n = LinkedList_Head(ll);
  ck_assert_int_eq(*n, 2);

  LLIterator_Next(ll_iter);

  // 2 -> 3 -> 4 ->
  //      |

  n = LLIterator_Remove(ll_iter);
  ck_assert_int_eq(*n, 3);

  // 2 -> 4 ->
  //      |

  n = LLIterator_Get(ll_iter);
  ck_assert_int_eq(*n, 4);

  n = LLIterator_Remove(ll_iter);
  ck_assert_int_eq(*n, 4);

  // 2 -> 
  //      |

  ck_assert(!LLIterator_IsValid(ll_iter));

  // check to make sure the list ended in the correct state
  n = LinkedList_Head(ll);
  ck_assert_int_eq(*n, 2);

  ck_assert_int_eq(LinkedList_Size(ll), 1);
}
END_TEST

Suite *basic_suite(void) {
  Suite *suite = suite_create("LinkedList Basic Suite");

  TCase *tc_basic = tcase_create("Basic Operations");
  tcase_add_checked_fixture(tc_basic, setup, teardown);

  tcase_add_test(tc_basic, size);
  tcase_add_test(tc_basic, is_empty);
  tcase_add_test(tc_basic, remove_empty);
  tcase_add_test(tc_basic, add_remove);
  tcase_add_test(tc_basic, head);

  TCase *tc_iterator = tcase_create("Iterator");
  tcase_add_checked_fixture(tc_iterator, setup, teardown);

  tcase_add_test(tc_iterator, iterator_get);
  tcase_add_test(tc_iterator, iterator_remove);

  suite_add_tcase(suite, tc_basic);
  suite_add_tcase(suite, tc_iterator);
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
