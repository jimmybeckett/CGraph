#include <stdlib.h>
#include <LinkedList.h>
#include <check.h>
#include <stdio.h>

// don't put actual pointers in this! they will leak :)
LinkedList *ll;

void setup(void) {
  ll = LinkedList_Allocate();
}

void teardown(void) {
  LinkedList_Free(ll, NULL);
}

START_TEST(size) {
  ck_assert_int_eq(LinkedList_Size(ll), 0);

  LinkedList_PushHead(ll, (void *) 1);
  ck_assert_int_eq(LinkedList_Size(ll), 1);

  LinkedList_PushTail(ll, (void *) 1);
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

  LinkedList_PushHead(ll, (void *) 1);

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
    LinkedList_PushHead(ll, (void *) ((long) i));
  }
  // 1 -> 2 -> ... -> 10

  for (int i = n + 1; i <= 2*n; i++) {
    LinkedList_PushTail(ll, (void *) ((long) i));
  }
  // 1 -> 2 -> ... -> 10 -> 11 -> 12 -> ... -> 20

  for (int i = 1; i <= 2*n; i++) {
    int j = (int) LinkedList_PopHead(ll);
    ck_assert_int_eq(j, i);
  }
}
END_TEST

START_TEST(peek_head) {
  ck_assert(LinkedList_PeekHead(ll) == NULL);

  LinkedList_PushHead(ll, (void *) 1);
  LinkedList_PushHead(ll, (void *) 2);

  int n = (int) LinkedList_PeekHead(ll);
  ck_assert_int_eq(n, 2);

  LinkedList_PopHead(ll);

  n = (int) LinkedList_PeekHead(ll);
  ck_assert_int_eq(n, 1);
}
END_TEST

START_TEST(peek_tail) {
  ck_assert(LinkedList_PeekTail(ll) == NULL);

  LinkedList_PushTail(ll, (void *) 1);
  LinkedList_PushTail(ll, (void *) 2);

  int n = (int) LinkedList_PeekTail(ll);
  ck_assert_int_eq(n, 2);

  LinkedList_PopHead(ll);

  n = (int) LinkedList_PeekTail(ll);
  ck_assert_int_eq(n, 2);
}
END_TEST

START_TEST(append) {
  LinkedList *ll2 = LinkedList_Allocate();

  LinkedList_Append(ll, ll2);
  ck_assert(LinkedList_IsEmpty(ll));
  ck_assert(LinkedList_IsEmpty(ll2));

  LinkedList_PushHead(ll, (void *) 2);
  LinkedList_PushHead(ll, (void *) 1);
  // ll: 1 -> 2
  
  LinkedList_Append(ll, ll2);
  ck_assert(LinkedList_Size(ll) == 2);
  ck_assert(LinkedList_Size(ll2) == 0);
  int *n = LinkedList_PeekHead(ll);
  ck_assert_int_eq(*n, 1);
  n = LinkedList_PeekTail(ll);
  ck_assert_int_eq(*n, 2);

  LinkedList_PushHead(ll2, (void *) 4);
  LinkedList_PushHead(ll2, (void *) 3);
  // ll2: 3 -> 4
  
  LinkedList_Append(ll, ll2);  // ll: 1 -> 2 -> 3 -> 4, ll2: (empty)
  ck_assert(LinkedList_Size(ll) == 4);
  ck_assert(LinkedList_Size(ll2) == 0);
  for (int i = 1; i <= 4; i++) {
    n = LinkedList_PopHead(ll);
    ck_assert_int_eq(*n, i);
  }

  LinkedList_Free(ll2, NULL);
}
END_TEST

void incrementInt(void *n) {
  *((int *) n) = *((int *) n) + 1;
}

START_TEST(test_free) {
  LinkedList *linked_list = LinkedList_Allocate();
  int n = 1;
  LinkedList_PushHead(linked_list, (void *) &n);

  LinkedList_Free(linked_list, incrementInt);
  ck_assert_int_eq(n, 2);
}
END_TEST

// n1 and n2 are really int64_t's put into void*'s
bool equalInt(void *n1, void *n2) {
  return (int64_t) n1 == (int64_t) n2;
}

START_TEST(test_find) {
  int64_t n = 10;
  int64_t output;

  ck_assert(!LinkedList_Find(ll, (void *) n, (Payload_t *) &output, equalInt));

  for (int64_t i = 0; i < n; i++) {
    LinkedList_PushHead(ll, (void *) i);
  }

  for (int64_t i = 0; i < n; i++) {
    ck_assert(LinkedList_Find(ll, (void *) i, (Payload_t *) &output, equalInt));
    ck_assert_int_eq(i, output);
  }

  ck_assert(!LinkedList_Find(ll, (void *) n, (Payload_t *) &output, equalInt));
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
  tcase_add_test(tc_basic, test_free);
  tcase_add_test(tc_basic, test_find);

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
