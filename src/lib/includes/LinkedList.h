#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdbool.h>
#include <stddef.h>

// A simple singly linked list implementation.
typedef struct linked_list_t LinkedList;

typedef void *Payload_t;

typedef void (LL_FreeFunction_t (void *payload));

typedef bool (LL_Comparator_t (void *payload, void *target));

// Allocates a new linked list with no elements and size 0.
// Returns NULL on failure.
LinkedList *LinkedList_Allocate();

// Calls free_function on every element in the list.
// If free_function is NULL then it isn't called (and runtime is O(1)).
// Runtime is O(size).
void LinkedList_Free(LinkedList *list, LL_FreeFunction_t *free_function);

// Returns the number of elements in the linked list.
size_t LinkedList_Size(LinkedList *list);

// Returns true if the linked list has size 0, and false otherwise.
bool LinkedList_IsEmpty(LinkedList *list);

// Pushes payload to the head of the linked list.
// Responsibility for deallocating payload is tranferred to the linked list.
// Returns true on success and false on failure.
bool LinkedList_PushHead(LinkedList *list, Payload_t payload);

// Pops and returns the head from the linked list and returns it to the caller.
// Responsibility for deallocating the returned payload is transferred to the caller.
Payload_t LinkedList_PopHead(LinkedList *list);

// Returns the payload stored in the head of the linked list.
// The payload remains the responsibility of the linked list to deallocate.
Payload_t LinkedList_PeekHead(LinkedList *list);

// Finds and returns through output the Payload_t closest to the head of the list where comparator(payload, target) 
// is true.
// The linked list retains the responsiblity for deallocating the payload returned through output.
// Returns true if such a payload was found, and false otherwise.
bool LinkedList_Find(LinkedList *list, Payload_t target, Payload_t *output, LL_Comparator_t *comparator);

// Finds and returns through output the Payload_t closest to the head of the list where comparator(payload, target) 
// is true.
// The returned payload is removed from the list.
// Responsiblity for deallocating the payload returned through output is transferred to the caller.
// Returns true if such a payload was found, and false otherwise.
bool LinkedList_RemoveIfPresent(LinkedList *list, Payload_t target, Payload_t *output, LL_Comparator_t *comparator);

/**************************************
 * Linked list iterator
**************************************/

// Used for iterating through a linked list from head -> tail.
typedef struct lliterator_t LLIterator;

// Allocates a new LLIterator.
// Returns NULL on failure.
LLIterator *LLIterator_Allocate(LinkedList *list);

// Frees an iterator.
void LLIterator_Free(LLIterator *iter);

// Returns true if the iterator is valid (not at the end of the list) and false otherwise.
bool LLIterator_IsValid(LLIterator *iter);

// If the iterator is valid, the current value of the iterator is assigned to output and true is returned.
// If the iterator isn't valid, false is returned.
bool LLIterator_Get(LLIterator *iter, Payload_t *output);

// Advances the iterator by 1. 
// Returns true if the iterator is valid and false otherwise.
bool LLIterator_Next(LLIterator *iter);

#endif  // LINKED_LIST_H_
