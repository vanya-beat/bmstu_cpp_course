<<<<<<< HEAD
add_subdirectory(task_basic_c)
add_subdirectory(bmstu_string)
add_subdirectory(bmstu_lets)
add_subdirectory(bmstu_stack)
add_subdirectory(bmstu_simple_vector)
add_subdirectory(bmstu_abstract_iterator)
add_subdirectory(bmstu_list)
add_subdirectory(bmstu_optional)
add_subdirectory(bmstu_map)
=======
cmake_minimum_required(VERSION 3.28)
project(bmstu_cpp_course)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        6910c9d9165801d8827d628cb72eb7ea9dd538c5
)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

file(WRITE ${CMAKE_SOURCE_DIR}/.gdbinit "source gdb_printer.py\n")
file(WRITE ${CMAKE_SOURCE_DIR}/.lldbinit "command script import lldb_printer.py\n")

if(UNIX)
    set(HOME_DIR "$ENV{HOME}")

    set(CREATE_SCRIPT "${CMAKE_BINARY_DIR}/create_debug_files.sh")
    file(WRITE ${CREATE_SCRIPT}
            "#!/bin/bash\n"
            "echo 'directory ${CMAKE_SOURCE_DIR}' > ${HOME_DIR}/.gdbinit\n"
            "echo 'add-auto-load-safe-path /' >> ${HOME_DIR}/.gdbinit\n"
            "echo 'set auto-load local-gdbinit on' >> ${HOME_DIR}/.gdbinit\n"
            "echo 'settings set target.load-cwd-lldbinit true' > ${HOME_DIR}/.lldbinit\n"
            "chmod 644 ${HOME_DIR}/.gdbinit ${HOME_DIR}/.lldbinit\n")

    execute_process(COMMAND chmod +x ${CREATE_SCRIPT})
    execute_process(COMMAND ${CREATE_SCRIPT} RESULT_VARIABLE result)

    if(NOT result EQUAL 0)
        message(WARNING "Failed to create debug files in home directory")
    endif()
endif()

configure_file(${CMAKE_SOURCE_DIR}/gdb_printer.py ${CMAKE_BINARY_DIR}/gdb_printer.py COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/.gdbinit ${CMAKE_BINARY_DIR}/.gdbinit COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/.lldbinit ${CMAKE_BINARY_DIR}/.lldbinit COPYONLY)

install(FILES ${CMAKE_SOURCE_DIR}/gdb_printer.py DESTINATION share/gdb/python)
install(FILES ${CMAKE_SOURCE_DIR}/.gdbinit DESTINATION share/gdb)
install(FILES ${CMAKE_SOURCE_DIR}/.lldbinit DESTINATION share/lldb)

add_subdirectory(tasks)
>>>>>>> 16c1abbf6b767830dc96cfb2fe94ba1f1c9d9093
