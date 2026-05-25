add_library(bsp OBJECT

)

target_compile_options(bsp PRIVATE -O2)

target_include_directories(bsp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_directories(bsp PUBLIC stm32cubemx azrtos::threadx)