find_package(Check REQUIRED)

include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)

# linked list test
add_executable(test_linkedlist test_linkedlist.c)
target_link_libraries(test_linkedlist linkedlist ${CHECK_LIBRARIES} pthread)

add_test(NAME test_linkedlist COMMAND test_linkedlist WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(test_linkedlist PROPERTIES TIMEOUT 30) 

# hash table test
add_executable(test_hashtable test_hashtable.c)
target_link_libraries(test_hashtable hashtable ${CHECK_LIBRARIES} pthread)

add_test(NAME test_hashtable COMMAND test_hashtable WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(test_hashtable PROPERTIES TIMEOUT 30)
