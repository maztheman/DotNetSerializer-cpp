find_package(fmt CONFIG REQUIRED)

add_library(dnscpp STATIC)

add_subdirectory(core)
add_subdirectory(dotnet)
add_subdirectory(types)
add_subdirectory(utils)

target_include_directories(dnscpp PUBLIC ".")
target_precompile_headers(dnscpp PRIVATE "pch.h")
target_link_libraries(dnscpp PUBLIC fmt::fmt)
