add_library(app OBJECT
    app_init.c
    car_control.c
)

target_include_directories(app PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(app PUBLIC stm32cubemx azrtos::threadx utils bsp CMSISDSP modules)