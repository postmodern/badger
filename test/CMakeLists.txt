enable_testing()

add_library(test STATIC test.c)

add_executable(test_slist_search test_slist_search.c)
target_link_libraries(test_slist_search test badger_core)
add_test(slist_search test_slist_search)
