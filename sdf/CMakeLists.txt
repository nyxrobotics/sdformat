add_subdirectory(1.0)
add_subdirectory(1.2)
add_subdirectory(1.3)
add_subdirectory(1.4)
add_subdirectory(1.5)
add_subdirectory(1.6)

add_custom_target(schema)
add_dependencies(schema schema1_6)