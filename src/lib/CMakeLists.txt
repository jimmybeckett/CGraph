add_library(hashtable HashTable.c)

# hashtable
target_include_directories(hashtable PUBLIC includes)
target_link_libraries(hashtable)
set_target_properties(hashtable PROPERTIES PUBLIC_HEADER includes/HashTable.h)

# install
install(TARGETS hashtable
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include
)
