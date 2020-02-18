#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "LinkedList.h"

typedef struct node_t {
  Payload_t payload;
  struct node_t *next;
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
    free(LinkedList_Pop(linked_list));
  }

  free(linked_list);
}

size_t LinkedList_Size(LinkedList *linked_list) {
  return linked_list->size;
}

int32_t LinkedList_IsEmpty(LinkedList *linked_list) {
  return linked_list->size == 0;
}

int32_t LinkedList_Push(LinkedList *linked_list, Payload_t payload) {
  Node *node = (Node*) malloc(sizeof(Node));
  if (node == NULL)  // check for malloc failure
    return 0;

  *node = (Node) {.payload = payload, .next = NULL };

  if (LinkedList_IsEmpty(linked_list)) {
    linked_list->head =  node;
  } else {
    node->next = linked_list->head;
    linked_list->head = node;
  }

  linked_list->size++;

  return 1;
}

Payload_t LinkedList_Pop(LinkedList *linked_list) {
  if (LinkedList_IsEmpty(linked_list))
    return NULL;

  Node *head = linked_list->head;

  linked_list->head = linked_list->head->next;

  linked_list->size--;

  Payload_t payload = head->payload;

  free(head);

  return payload;
}

