add_library(linkedlist LinkedList.c)

target_include_directories(linkedlist PUBLIC includes)
set_target_properties(linkedlist PROPERTIES PUBLIC_HEADER includes/LinkedList.h)

install(TARGETS linkedlist
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include
)
