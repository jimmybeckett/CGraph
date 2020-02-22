#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <LinkedList.h>

/**************************************
 * Structs
**************************************/

typedef struct node_t {
  Payload_t payload;
  struct node_t *next;
} Node;

struct linked_list_t {  // LinkedList, declared in LinkedList.h
  Node *head;
  size_t size;
};

struct lliterator_t {  // LLIterator, declared in LinkedList.h
  LinkedList *list;
  Node *curr;
};

/**************************************
 * Functions declared in LinkedList.h
**************************************/

LinkedList *LinkedList_Allocate() {
  LinkedList *list = (LinkedList*) malloc(sizeof(LinkedList));
  if (list == NULL) return NULL;

  *list = (LinkedList) {.head = NULL, .size = 0};
  return list;
}

void LinkedList_Free(LinkedList *list, LL_FreeFunction_t *free_function) {
  if (free_function != NULL) {
    while (!LinkedList_IsEmpty(list)) {
      free_function(LinkedList_PopHead(list));
    }
  }

  free(list);
}

size_t LinkedList_Size(LinkedList *list) {
  return list->size;
}

bool LinkedList_IsEmpty(LinkedList *list) {
  return list->size == 0;
}

bool LinkedList_PushHead(LinkedList *list, Payload_t payload) {
  Node *node = (Node *) malloc(sizeof(Node));
  if (node == NULL) return false;

  *node = (Node) {.payload = payload, .next = list->head};
  list->head = node;
  list->size++;

  return true;
}

Payload_t LinkedList_PopHead(LinkedList *list) {
  if (LinkedList_IsEmpty(list)) return NULL;

  Node *old_head = list->head;
  list->head = list->head->next;
  Payload_t payload = old_head->payload;
  free(old_head);

  list->size--;

  return payload;
}

Payload_t LinkedList_PeekHead(LinkedList *list) {
  if (LinkedList_IsEmpty(list)) return NULL;

  return list->head->payload;
}

bool LinkedList_Find(LinkedList *list, Payload_t target, Payload_t *output, LL_Comparator_t *comparator) {
  Node *curr = list->head;
  while (curr != NULL) {
    if (comparator(curr->payload, target)) {
      *output = curr->payload;
      return true;
    }
    curr = curr->next;
  }
  return false;
}

bool LinkedList_RemoveIfPresent(LinkedList *list, Payload_t target, Payload_t *output, LL_Comparator_t *comparator) {
  if (LinkedList_IsEmpty(list)) return false;

  if (comparator(list->head->payload, target)) {
    *output = list->head->payload;
    Node *old_head = list->head;
    list->head = list->head->next;
    free(old_head);
    list->size--;
    return true;
  }

  Node *curr = list->head;
  while (curr->next != NULL) {
    if (comparator(curr->next->payload, target)) {
      *output = curr->next->payload;
      Node *old_next = curr->next;
      curr->next = curr->next->next;
      free(old_next);
      list->size--;
      return true;
    }
    curr = curr->next;
  }

  return false;
}

/**************************************
 * Linked list iterator
**************************************/

LLIterator *LLIterator_Allocate(LinkedList *list) {
  LLIterator *iter = (LLIterator *) malloc(sizeof(LLIterator));
  if (iter == NULL) return NULL;
  *iter = (LLIterator) { .list = list, .curr = list->head };
  return iter;
}

void LLIterator_Free(LLIterator *iter) {
  free(iter);
}

bool LLIterator_IsValid(LLIterator *iter) {
  return iter->curr != NULL;
}

bool LLIterator_Get(LLIterator *iter, Payload_t *output) {
  if (LLIterator_IsValid(iter)) {
    *output = iter->curr->payload;
    return true;
  }
  return false;
}

bool LLIterator_Next(LLIterator *iter) {
  if (LLIterator_IsValid(iter)) {
    iter->curr = iter->curr->next;
    return true;
  }
  return false;
}
