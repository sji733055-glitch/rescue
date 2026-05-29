add_library(modules OBJECT 
    module_init.c
    REMOTE/module_remote.c
    REMOTE/PS2/ps2.c
    MOTOR/motor_bace.c
    MOTOR/SERVO/motor_servo.c
)
target_compile_options(modules PRIVATE -O2 -include module_config.h)

target_include_directories(modules PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/REMOTE
    ${CMAKE_CURRENT_SOURCE_DIR}/REMOTE/PS2
    ${CMAKE_CURRENT_SOURCE_DIR}/MOTOR
    ${CMAKE_CURRENT_SOURCE_DIR}/MOTOR/SERVO

)

target_link_libraries(modules PUBLIC stm32cubemx azrtos::threadx utils bsp)