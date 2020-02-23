find_package(Check REQUIRED)

include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)

# hash table test
add_executable(test_hashtable test_hashtable.c)
target_link_libraries(test_hashtable hashtable ${CHECK_LIBRARIES} pthread)

add_test(NAME test_hashtable COMMAND test_hashtable WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(test_hashtable PROPERTIES TIMEOUT 30)
