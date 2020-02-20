#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdbool.h>
#include <stddef.h>

// A simple and fast linked list implementation.
// All operations (except for free) are O(1).
typedef struct linked_list_t LinkedList;

typedef void *Payload_t;

// Allocates a new linked list with no elements and size 0.
// Returns NULL on failure.
LinkedList *LinkedList_Allocate();

// Frees the linked list and all elements it contains.
// O(size)
void LinkedList_Free(LinkedList *linked_list);

// Returns the number of elements in the linked list.
size_t LinkedList_Size(LinkedList *linked_list);

// Returns true if the linked list has size 0, and false otherwise.
bool LinkedList_IsEmpty(LinkedList *linked_list);

// Pushes payload to the head of the linked list.
// Responsibility for deallocating payload is tranferred to the linked list.
// Returns true on success and false on failure.
bool LinkedList_PushHead(LinkedList *linked_list, Payload_t payload);

// Pops and returns the head from the linked list and returns it to the caller.
// Responsibility for deallocating the returned payload is transferred to the caller.
Payload_t LinkedList_PopHead(LinkedList *linked_list);

// Returns the payload stored in the head of the linked list.
// The payload remains the responsibility of the linked list to deallocate.
Payload_t LinkedList_Head(LinkedList *linked_list);

// Pushes payload to the tail of the linked list.
// Responsibility for deallocating payload is tranferred to the linked list.
// Returns true on success and false on failure.
bool LinkedList_PushTail(LinkedList *linked_list, Payload_t payload);

// Returns the payload stored in the tail of the linked list.
// The payload remains the responsiblity of the linked list to deallocate.
Payload_t LinkedList_Tail(LinkedList *linked_list);

#endif  // LINKED_LIST_H_
