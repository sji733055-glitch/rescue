add_library(modules OBJECT 
    module_init.c
    REMOTE/module_remote.c
    REMOTE/PS2/ps2.c
    MOTOR/motor_base.c
    MOTOR/SERVO/motor_servo.c
    algorithm/user_lib.c
    algorithm/pid.c
)
target_compile_options(modules PRIVATE -O2)

target_include_directories(modules PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/REMOTE
    ${CMAKE_CURRENT_SOURCE_DIR}/REMOTE/PS2
    ${CMAKE_CURRENT_SOURCE_DIR}/MOTOR
    ${CMAKE_CURRENT_SOURCE_DIR}/MOTOR/SERVO
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithm
)

target_link_libraries(modules PUBLIC stm32cubemx azrtos::threadx utils bsp CMSISDSP)