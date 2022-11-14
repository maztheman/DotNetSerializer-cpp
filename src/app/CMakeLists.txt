find_package(fmt CONFIG REQUIRED)

SET(SRCS 
    main.cpp
)

add_executable(example ${SRCS})

target_precompile_headers(example PRIVATE "pch.h")
target_link_libraries(example PRIVATE dnscpp fmt::fmt)