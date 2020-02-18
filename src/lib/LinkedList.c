#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "LinkedList.h"

typedef struct node_t {
  Payload_t payload;
  struct node_t *next;
  struct node_t *prev;
} Node;

struct linked_list_t {  // LinkedList
  Node *head;
  size_t size;
};

LinkedList *LinkedList_Allocate() {
  LinkedList *linked_list = (LinkedList*) malloc(sizeof(LinkedList));
  if (linked_list == NULL)  // check for malloc failure
    return 0;
  *linked_list = (LinkedList) {.head = NULL, .size = 0};

  return linked_list;
}

void LinkedList_Free(LinkedList *linked_list) {
  while (!LinkedList_IsEmpty(linked_list)) {
    free(LinkedList_Remove(linked_list));
  }

  free(linked_list);
}

size_t LinkedList_Size(LinkedList *linked_list) {
  return linked_list->size;
}

int32_t LinkedList_IsEmpty(LinkedList *linked_list) {
  return linked_list->size == 0;
}

int32_t LinkedList_Add(LinkedList *linked_list, Payload_t payload) {
  Node *node = (Node*) malloc(sizeof(Node));
  if (node == NULL)  // check for malloc failure
    return 0;

  *node = (Node) {.payload = payload, .next = linked_list->head, .prev = NULL };

  if (linked_list->head != NULL) {
    linked_list->head->prev = node;
  }
  linked_list->head = node;

  linked_list->size++;

  return 1;
}

Payload_t LinkedList_Remove(LinkedList *linked_list) {
  if (LinkedList_IsEmpty(linked_list))
    return NULL;

  Node *old_head = linked_list->head;

  linked_list->head = linked_list->head->next;
  if (linked_list->head != NULL) {
    linked_list->head->prev = NULL;
  }

  linked_list->size--;

  Payload_t payload = old_head->payload;

  free(old_head);

  return payload;
}

Payload_t LinkedList_Head(LinkedList *linked_list) {
  if (LinkedList_IsEmpty(linked_list))
    return NULL;
  return linked_list->head->payload;
}

struct lliterator_t {  // LLIterator
  LinkedList *list;
  Node *curr;
};

LLIterator *LLIterator_Allocate(LinkedList *linked_list) {
  LLIterator *lliterator = (LLIterator*) malloc(sizeof(LLIterator));
  if (lliterator == NULL)
    return NULL;

  *lliterator = (LLIterator) { .list = linked_list, .curr = linked_list->head };
  return lliterator;
}

void LLIterator_Free(LLIterator *lliterator) {
  free(lliterator);
}

int32_t LLIterator_IsValid(LLIterator *lliterator) {
  return lliterator->curr != NULL;
}

int32_t LLIterator_Next(LLIterator *lliterator) {
  lliterator->curr = lliterator->curr->next;
  return LLIterator_IsValid(lliterator);
}

Payload_t LLIterator_Get(LLIterator *lliterator) {
  return lliterator->curr->payload;
}

Payload_t LLIterator_Remove(LLIterator *lliterator) {
  if (lliterator->curr->next != NULL) {
    lliterator->curr->next->prev = lliterator->curr->prev;
  }

  if (lliterator->curr == lliterator->list->head) {
    lliterator->list->head = lliterator->curr->next;
  } else {  // lliterator->curr->prev != NULL
    lliterator->curr->prev->next = lliterator->curr->next;
  }

  Node *curr = lliterator->curr;
  Payload_t payload = curr->payload;

  lliterator->curr = lliterator->curr->next;

  free(curr);

  lliterator->list->size--;

  return payload;
}
