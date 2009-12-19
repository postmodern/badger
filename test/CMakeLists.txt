enable_testing()

add_library(test STATIC test.c)

add_executable(test_list_search test_list_search.c)
target_link_libraries(test_list_search test ronin_rat)
add_test(list_search test_list_search)
