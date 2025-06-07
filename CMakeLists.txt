cmake_minimum_required(VERSION 3.28)
project(bmstu_cpp_course)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 6910c9d9165801d8827d628cb72eb7ea9dd538c5
)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

if(UNIX)
    set(PRINTERS_DIR ${CMAKE_BINARY_DIR}/printers)
    file(MAKE_DIRECTORY ${PRINTERS_DIR})

    configure_file(
        ${CMAKE_SOURCE_DIR}/bmstu_list_printer_gdb.py
        ${PRINTERS_DIR}/bmstu_list_printer_gdb.py
        COPYONLY
    )
    configure_file(
        ${CMAKE_SOURCE_DIR}/bmstu_list_printer_lldb.py
        ${PRINTERS_DIR}/bmstu_list_printer_lldb.py
        COPYONLY
    )
    configure_file(
        ${CMAKE_SOURCE_DIR}/gdb_printer.py
        ${PRINTERS_DIR}/gdb_printer.py
        COPYONLY
    )

    file(WRITE ${CMAKE_BINARY_DIR}/.gdbinit
        "python\n"
        "import sys\n"
        "sys.path.insert(0, '${PRINTERS_DIR}')\n"
        "from bmstu_list_printer_gdb import *\n"
        "end\n"
        "source ${PRINTERS_DIR}/gdb_printer.py\n"
    )

    file(WRITE ${CMAKE_BINARY_DIR}/.lldbinit
        "command script import ${PRINTERS_DIR}/bmstu_list_printer_lldb.py\n"
    )

    set(HOME_DIR "$ENV{HOME}")
    set(CREATE_SCRIPT "${CMAKE_BINARY_DIR}/create_debug_files.sh")
    file(WRITE ${CREATE_SCRIPT}
        "#!/bin/bash\n"
        "echo 'directory ${CMAKE_SOURCE_DIR}' > ${HOME_DIR}/.gdbinit\n"
        "echo 'add-auto-load-safe-path /' >> ${HOME_DIR}/.gdbinit\n"
        "echo 'set auto-load local-gdbinit on' >> ${HOME_DIR}/.gdbinit\n"
        "echo 'settings set target.load-cwd-lldbinit true' > ${HOME_DIR}/.lldbinit\n"
        "chmod 644 ${HOME_DIR}/.gdbinit ${HOME_DIR}/.lldbinit\n"
    )

    execute_process(COMMAND chmod +x ${CREATE_SCRIPT})
    execute_process(COMMAND ${CREATE_SCRIPT} RESULT_VARIABLE result)

    if(NOT result EQUAL 0)
        message(WARNING "Failed to create debug files in home directory")
    endif()
endif()

if(WIN32)
    set(NATVIS_FILE "bmstu_list_printer_msvc.natvis")
    
    configure_file(
        ${CMAKE_SOURCE_DIR}/${NATVIS_FILE}
        ${CMAKE_BINARY_DIR}/${NATVIS_FILE}
        COPYONLY
    )
    
    if(MSVC)
        file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/${NATVIS_FILE}" NATVIS_PATH)
        set(CMAKE_VS_GLOBAL_VS_DEBUGGER_NATVIS_FILE ${NATVIS_PATH})
    endif()
    
    if(EXISTS "${CMAKE_SOURCE_DIR}/.idea")
        configure_file(
            ${CMAKE_SOURCE_DIR}/${NATVIS_FILE}
            ${CMAKE_SOURCE_DIR}/.idea/${NATVIS_FILE}
            COPYONLY
        )
    endif()
endif()

install(
    FILES 
    ${CMAKE_SOURCE_DIR}/bmstu_list_printer_gdb.py
    ${CMAKE_SOURCE_DIR}/bmstu_list_printer_lldb.py
    ${CMAKE_SOURCE_DIR}/gdb_printer.py
    DESTINATION share/gdb/python
)

if(WIN32)
    install(
        FILES ${CMAKE_SOURCE_DIR}/${NATVIS_FILE}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/natvis
    )
endif()

add_subdirectory(tasks)