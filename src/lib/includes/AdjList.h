#ifndef ADJ_LIST_H_ 
#define ADJ_LIST_H_

#include <stdint.h>
#include "Vertex.h"

typedef struct adjlist_t AdjList;

int32_t AdjList_AddVertex(Vertex vertex);

#endif  // ADJ_LIST_H_
