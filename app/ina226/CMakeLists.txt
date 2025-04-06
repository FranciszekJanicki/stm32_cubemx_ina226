add_library(ina226 STATIC)

target_sources(ina226 PRIVATE 
    "ina226.cpp"
)

target_include_directories(ina226 PUBLIC 
    ${CMAKE_SOURCE_DIR}/app/stm32_utility
)

target_link_libraries(ina226 PUBLIC
    stm32cubemx
    stm32_utility
)

target_compile_options(ina226 PUBLIC
    -std=c++23
    -Wall
    -Wextra
    -Wconversion
    -Wshadow
    -Wpedantic
    -Wnarrowing
    -Waddress
    -pedantic
    -Wdeprecated
    -Wsign-conversion
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wnull-dereference
    -Wdouble-promotion
    -Wimplicit-fallthrough
    -Wcast-align
    -fconcepts
)
