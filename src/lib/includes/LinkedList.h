#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdint.h>
#include <stddef.h>

typedef struct linked_list_t LinkedList;

typedef void* Payload_t;

LinkedList *LinkedList_Allocate();

void LinkedList_Free(LinkedList *linked_list);

size_t LinkedList_Size(LinkedList *linked_list);

int32_t LinkedList_IsEmpty(LinkedList *linked_list);

int32_t LinkedList_Push(LinkedList *linked_list, Payload_t payload);

Payload_t LinkedList_Pop(LinkedList *linked_list);

Payload_t LinkedList_Peek(LinkedList *linked_list);

#endif  // LINKED_LIST_H_
