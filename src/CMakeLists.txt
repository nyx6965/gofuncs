add_library(coralis_lib
    builtin.c
    helper.c
    utils.c
    args.c
)

add_executable(coralis main.c)

target_link_libraries(coralis coralis_lib)
