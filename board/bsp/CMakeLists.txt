add_library(bsp OBJECT
    bsp_init.c
    GPIO/bsp_gpio.c
    PWM/bsp_pwm.c
    UART/bsp_uart.c
    DWT/bsp_dwt.c
    SPI/bsp_softspi.c
)

target_compile_options(bsp PRIVATE -O2)

target_include_directories(bsp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/GPIO
    ${CMAKE_CURRENT_SOURCE_DIR}/PWM
    ${CMAKE_CURRENT_SOURCE_DIR}/UART
    ${CMAKE_CURRENT_SOURCE_DIR}/DWT
    ${CMAKE_CURRENT_SOURCE_DIR}/SPI
)

target_link_libraries(bsp PUBLIC stm32cubemx azrtos::threadx utils)