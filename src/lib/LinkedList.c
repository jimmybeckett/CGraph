#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <LinkedList.h>

typedef struct node_t {
  Payload_t payload;
  struct node_t *next;
} Node;

struct linked_list_t {  // LinkedList
  Node *head;
  Node *tail;
  size_t size;
};

LinkedList *LinkedList_Allocate() {
  LinkedList *list = (LinkedList*) malloc(sizeof(LinkedList));
  if (list == NULL) return NULL;

  *list = (LinkedList) {.head = NULL, .tail = NULL, .size = 0};
  return list;
}

void LinkedList_Free(LinkedList *list, FreeFunction_t free_function) {
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

  if (list->size == 0) {
    list->head = list->tail = node;
  } else {
    list->head = node;
  }

  list->size++;

  return true;
}

Payload_t LinkedList_PopHead(LinkedList *list) {
  if (LinkedList_IsEmpty(list)) return NULL;

  Node *old_head = list->head;
  
  if (list->size == 1) {
    list->head = list->tail = NULL;
  } else {
    list->head = list->head->next;
  }

  list->size--;

  Payload_t payload = old_head->payload;

  free(old_head);

  return payload;
}

Payload_t LinkedList_PeekHead(LinkedList *list) {
  if (LinkedList_IsEmpty(list)) return NULL;

  return list->head->payload;
}

bool LinkedList_PushTail(LinkedList *list, Payload_t payload) {
  Node *node = (Node *) malloc(sizeof(Node));
  if (node == NULL) return false;

  *node = (Node) {.payload = payload, .next = NULL};

  if (list->size == 0) {
    list->head = list->tail = node;
  } else {
    list->tail->next = node;
    list->tail = node;
  }

  list->size++;

  return true;
}

Payload_t LinkedList_PeekTail(LinkedList *list) {
  if (LinkedList_IsEmpty(list)) return NULL;

  return list->tail->payload;
}

void LinkedList_Append(LinkedList *dest, LinkedList *src) {
  if (src->size == 0) return;

  if (dest->size == 0) {
    dest->head = src->head;
  } else {
    dest->tail->next = src->head;
  }

  dest->tail = src->tail;
  dest->size += src->size;
  
  src->head = src->tail = NULL;
  src->size = 0;
}

bool LinkedList_Find(LinkedList *list, Payload_t target, Payload_t *output, Comparator_t comparator) {
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
