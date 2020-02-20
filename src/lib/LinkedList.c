#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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
  LinkedList *linked_list = (LinkedList*) malloc(sizeof(LinkedList));
  if (linked_list == NULL)  // check for malloc failure
    return NULL;

  *linked_list = (LinkedList) {.head = NULL, .tail = NULL, .size = 0};
  return linked_list;
}

void LinkedList_Free(LinkedList *linked_list) {
  while (!LinkedList_IsEmpty(linked_list)) {
    free(LinkedList_PopHead(linked_list));
  }

  free(linked_list);
}

size_t LinkedList_Size(LinkedList *linked_list) {
  return linked_list->size;
}

bool LinkedList_IsEmpty(LinkedList *linked_list) {
  return linked_list->size == 0;
}

bool LinkedList_PushHead(LinkedList *linked_list, Payload_t payload) {
  Node *node = (Node *) malloc(sizeof(Node));
  if (node == NULL)  // check for malloc failure
    return false;

  *node = (Node) {.payload = payload, .next = linked_list->head};

  if (linked_list->size == 0) {
    linked_list->head = linked_list->tail = node;
  } else {
    linked_list->head = node;
  }

  linked_list->size++;

  return true;
}

Payload_t LinkedList_PopHead(LinkedList *linked_list) {
  if (LinkedList_IsEmpty(linked_list))
    return NULL;

  Node *old_head = linked_list->head;
  
  if (linked_list->size == 1) {
    linked_list->head = linked_list->tail = NULL;
  } else {
    linked_list->head = linked_list->head->next;
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

bool LinkedList_PushTail(LinkedList *linked_list, Payload_t payload) {
  Node *node = (Node *) malloc(sizeof(Node));
  if (node == NULL)  // check for malloc failure
    return false;

  *node = (Node) {.payload = payload, .next = NULL};

  if (linked_list->size == 0) {
    linked_list->head = linked_list->tail = node;
  } else {
    linked_list->tail->next = node;
    linked_list->tail = node;
  }

  linked_list->size++;

  return true;
}

Payload_t LinkedList_Tail(LinkedList *linked_list) {
  if (LinkedList_IsEmpty(linked_list))
    return NULL;
  return linked_list->tail->payload;
}
