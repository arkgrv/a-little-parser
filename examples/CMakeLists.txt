set(EXEC little-parser)
set(LIB ${CMAKE_PROJECT_NAME}-dev)
add_executable(${EXEC} LittleParser.cpp)
target_link_libraries(${EXEC} PUBLIC ${LIB})
