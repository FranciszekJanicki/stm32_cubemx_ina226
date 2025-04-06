add_library(stm32_utility STATIC)

target_sources(stm32_utility PRIVATE 
    "gpio.cpp"
    "i2c_device.cpp"
    "spi_device.cpp"
    "ow_device.cpp"
    "pwm_device.cpp"
    "cnt_device.cpp"
)

target_include_directories(stm32_utility PUBLIC 
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
)

target_link_libraries(stm32_utility PUBLIC
    stm32cubemx
)

target_compile_options(stm32_utility PUBLIC
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
