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

configure_file(${CMAKE_SOURCE_DIR}/gdb_printer.py ${CMAKE_BINARY_DIR}/gdb_printer.py COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/bmstu_list_printer_gdb.py ${CMAKE_BINARY_DIR}/bmstu_list_printer_gdb.py COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/bmstu_list_printer_lldb.py ${CMAKE_BINARY_DIR}/bmstu_list_printer_lldb.py COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/bmstu_list_printer_msvc.natvis ${CMAKE_BINARY_DIR}/bmstu_list_printer_msvc.natvis COPYONLY)

file(WRITE ${CMAKE_BINARY_DIR}/.gdbinit
    "source gdb_printer.py\n"
    "source bmstu_list_printer_gdb.py\n"
)

file(WRITE ${CMAKE_BINARY_DIR}/.lldbinit
    "command script import lldb_printer.py\n"
    "command script import bmstu_list_printer_lldb.py\n"
)

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

install(FILES 
    ${CMAKE_SOURCE_DIR}/gdb_printer.py
    ${CMAKE_SOURCE_DIR}/bmstu_list_printer_gdb.py
    DESTINATION share/gdb/python)
    
install(FILES 
    ${CMAKE_SOURCE_DIR}/lldb_printer.py
    ${CMAKE_SOURCE_DIR}/bmstu_list_printer_lldb.py
    DESTINATION share/lldb)

install(FILES ${CMAKE_BINARY_DIR}/.gdbinit DESTINATION share/gdb)
install(FILES ${CMAKE_BINARY_DIR}/.lldbinit DESTINATION share/lldb)


install(FILES 
    ${CMAKE_SOURCE_DIR}/bmstu_list_printer_msvc.natvis
    DESTINATION share/msvc/natvis)

if(MSVC)
    file(GLOB_RECURSE PROJECT_TARGETS "tasks/CMakeLists.txt")
    
    foreach(target_file ${PROJECT_TARGETS})
        get_filename_component(target_dir ${target_file} DIRECTORY)
        add_subdirectory(${target_dir})
        
        get_property(targets_in_dir DIRECTORY ${target_dir} PROPERTY BUILDSYSTEM_TARGETS)
        
        foreach(target ${targets_in_dir})
            target_sources(${target} PRIVATE ${CMAKE_BINARY_DIR}/bmstu_list_printer_msvc.natvis)
            set_source_files_properties(${CMAKE_BINARY_DIR}/bmstu_list_printer_msvc.natvis 
                PROPERTIES VS_TOOL_OVERRIDE "natvis")
        endforeach()
    endforeach()
else()
    add_subdirectory(tasks)
endif()