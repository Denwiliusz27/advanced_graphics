add_executable (ZGK_1 "ZGK_1.cpp" "ZGK_1.h" "Time.hpp" "Time.cpp" "Input.cpp" "Input.hpp")

find_package(OPENSCENEGRAPH)

target_link_libraries(ZGK_1 PRIVATE OPENSCENEGRAPH::OPENSCENEGRAPH)

set_property(TARGET ZGK_1 PROPERTY CXX_STANDARD 20)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(ZGK_1 copy_assets)