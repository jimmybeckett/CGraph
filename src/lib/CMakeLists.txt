add_library(linkedlist LinkedList.c)
add_library(hashtable HashTable.c)

# linked list
target_include_directories(linkedlist PUBLIC includes)
set_target_properties(linkedlist PROPERTIES PUBLIC_HEADER includes/LinkedList.h)

# hashtable
target_include_directories(hashtable PUBLIC includes)
target_link_libraries(hashtable linkedlist)
set_target_properties(hashtable PROPERTIES PUBLIC_HEADER includes/HashTable.h)

# install
install(TARGETS linkedlist hashtable
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include
)
