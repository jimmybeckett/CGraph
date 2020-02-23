find_package(Check REQUIRED)

include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)

# hash table performance test
add_executable(test_hashtable_performance test_hashtable_performance.c)
target_link_libraries(test_hashtable_performance hashtable ${CHECK_LIBRARIES} pthread)
add_test(NAME test_hashtable_performance COMMAND test_hashtable_performance WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)

# hash table test
add_executable(test_hashtable test_hashtable.c)
target_link_libraries(test_hashtable hashtable ${CHECK_LIBRARIES} pthread)

add_test(NAME test_hashtable COMMAND test_hashtable WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(test_hashtable PROPERTIES TIMEOUT 30)
