#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdint.h>
#include <stddef.h>

/**************************************
 * Linked List
**************************************/

typedef struct linked_list_t LinkedList;

typedef void* Payload_t;

LinkedList *LinkedList_Allocate();

void LinkedList_Free(LinkedList *linked_list);

size_t LinkedList_Size(LinkedList *linked_list);

int32_t LinkedList_IsEmpty(LinkedList *linked_list);

int32_t LinkedList_Add(LinkedList *linked_list, Payload_t payload);

Payload_t LinkedList_Remove(LinkedList *linked_list);

Payload_t LinkedList_Head(LinkedList *linked_list);

/**************************************
 * Linked List Iterator
**************************************/

typedef struct lliterator_t LLIterator;

LLIterator *LLIterator_Allocate(LinkedList *linked_list);

void LLIterator_Free(LLIterator *lliterator);

int32_t LLIterator_IsValid(LLIterator *lliterator);

int32_t LLIterator_Next(LLIterator *lliterator);

Payload_t LLIterator_Get(LLIterator *lliterator);

Payload_t LLIterator_Remove(LLIterator *lliterator);

#endif  // LINKED_LIST_H_
