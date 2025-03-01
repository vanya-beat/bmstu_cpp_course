message(STATUS "Running tasks/basic_c/CMakeLists.txt")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
get_filename_component(NAME_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR} NAME)
#glob all path to folder in task_* an
file(GLOB TASKS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/task_*)
#find all files in folder with name *.c and *.h and *.cpp *.hpp and add them to the project
foreach(TASK ${TASKS})
    file(GLOB_RECURSE SOURCES  ${CMAKE_CURRENT_SOURCE_DIR}/${TASK} ${CMAKE_CURRENT_SOURCE_DIR}/${TASK}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/${TASK}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/${TASK}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/${TASK}/*.hpp)
endforeach ()
message(STATUS "TASKS: ${TASKS}")
message(STATUS "SOURCES: ${SOURCES}")
add_executable(${NAME_EXECUTABLE} ${SOURCES})
target_link_libraries(
        ${NAME_EXECUTABLE}
        GTest::gtest_main
)