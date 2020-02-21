#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdbool.h>
#include <stddef.h>

// A simple singly linked list implementation.
// All operations (except for free) are O(1).
typedef struct linked_list_t LinkedList;

typedef void *Payload_t;

typedef void (FreeFunction_t (void *payload));

typedef bool (Comparator_t (void *payload, void *target));

// Allocates a new linked list with no elements and size 0.
// Returns NULL on failure.
LinkedList *LinkedList_Allocate();

// Calls free_function on every element in the list.
// If free_function is NULL then it isn't called (and runtime is O(1)).
// Runtime is O(size).
void LinkedList_Free(LinkedList *list, FreeFunction_t free_function);

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

// Pushes payload to the tail of the linked list.
// Responsibility for deallocating payload is tranferred to the linked list.
// Returns true on success and false on failure.
bool LinkedList_PushTail(LinkedList *list, Payload_t payload);

// Returns the payload stored in the tail of the linked list.
// The payload remains the responsiblity of the linked list to deallocate.
Payload_t LinkedList_PeekTail(LinkedList *list);

// Appends the contents from src onto the tail of destination, leaving src empty.
void LinkedList_Append(LinkedList *dest, LinkedList *src);

// Finds and returns through output the Payload_t closest to the head of the list where comparator(payload, target) 
// is true.
// The linked list retains the esponsiblity for deallocating the payload returned through output.
// Returns true if such a payload was found, and false otherwise.
bool LinkedList_Find(LinkedList *list, Payload_t target, Payload_t *output, Comparator_t comparator);

#endif  // LINKED_LIST_H_
