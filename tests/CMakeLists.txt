add_executable(test_args
    test_args.c
    test_main.c
)

target_link_libraries(test_args coralis_lib)

add_test(NAME args_scanner_tests COMMAND test_args)
