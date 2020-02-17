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
  Node *tail;
  size_t size;
};

LinkedList *LinkedList_Allocate() {
  LinkedList *linked_list = (LinkedList*) malloc(sizeof(LinkedList));
  if (linked_list == NULL)  // check for malloc failure
    return 0;
  *linked_list = (LinkedList) {.head = NULL, .tail = NULL, .size = 0};

  return linked_list;
}

void LinkedList_Free(LinkedList *linked_list) {
  while (!LinkedList_IsEmpty(linked_list)) {
    Payload_t payload = LinkedList_PopHead(linked_list);
    free(payload);
  }

  free(linked_list);
}

size_t LinkedList_Size(LinkedList *linked_list) {
  return linked_list->size;
}

int32_t LinkedList_IsEmpty(LinkedList *linked_list) {
  return linked_list->size == 0;
}

int32_t LinkedList_PushHead(LinkedList *linked_list, Payload_t payload) {
  Node *node = (Node*) malloc(sizeof(Node));
  if (node == NULL)  // check for malloc failure
    return 0;

  *node = (Node) {.payload = payload, .next = NULL, .prev = NULL};

  if (LinkedList_IsEmpty(linked_list)) {
    linked_list->head = linked_list->tail = node;
  } else {
    node->next = linked_list->head;
    linked_list->head->prev = node;
    linked_list->head = node;
  }

  linked_list->size++;

  return 1;
}

Payload_t LinkedList_PopHead(LinkedList *linked_list) {
  if (LinkedList_IsEmpty(linked_list))
    return NULL;

  Node *head = linked_list->head;

  linked_list->head = linked_list->head->next;
  if (linked_list->head == NULL) {
    linked_list->tail = NULL;
  } else {
    linked_list->head->prev = NULL;
  }

  linked_list->size--;

  Payload_t payload = head->payload;

  free(head);

  return payload;
}

int32_t LinkedList_PushTail(LinkedList *linked_list, Payload_t payload) {
  Node *node = (Node*) malloc(sizeof(Node));
  if (node == NULL)  // check for malloc failure
    return 0;

  *node = (Node) {.payload = payload, .next = NULL, .prev = NULL};

  if (LinkedList_IsEmpty(linked_list)) {
    linked_list->head = linked_list->tail = node;
  } else {
    node->prev = linked_list->tail;
    linked_list->tail->next = node;
    linked_list->tail = node;
  }

  linked_list->size++;

  return 1;
}

Payload_t LinkedList_PopTail(LinkedList *linked_list) {
  if (LinkedList_IsEmpty(linked_list))
    return NULL;

  Node *tail = linked_list->tail;

  linked_list->tail = linked_list->tail->prev;
  if (linked_list->tail == NULL) {
    linked_list->head = NULL;
  } else {
    linked_list->tail->next = NULL;
  }

  linked_list->size--;

  Payload_t payload = tail->payload;

  free(tail);

  return payload;
}

