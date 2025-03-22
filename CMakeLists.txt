cmake_minimum_required(VERSION 3.28)
project(bmstu_cpp_course)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)


set(LLDBINIT_FILE ${CMAKE_SOURCE_DIR}/.lldbinit)
set(LLDBINIT_CONTENT "command script import lldb_printer.py")

if (UNIX)
    set(HOME_DIR $ENV{HOME})
    set(LLDBINIT_FILE "${HOME_DIR}/.lldbinit")
    set(LLDBINIT_CONTENT "settings set target.load-cwd-lldbinit true")

    if (EXISTS ${LLDBINIT_FILE})
        file(READ ${LLDBINIT_FILE} LLDBINIT_EXISTING_CONTENT)
        if (NOT "${LLDBINIT_EXISTING_CONTENT}" MATCHES "${LLDBINIT_CONTENT}")
            file(APPEND ${LLDBINIT_FILE} "${LLDBINIT_CONTENT}\n")
            message(STATUS "Добавлено '${LLDBINIT_CONTENT}' в .lldbinit")
        else()
            message(STATUS "Строка '${LLDBINIT_CONTENT}' уже присутствует в .lldbinit")
        endif()
    else()
        file(WRITE ${LLDBINIT_FILE} "${LLDBINIT_CONTENT}\n")
        message(STATUS "Файл .lldbinit создан с содержимым '${LLDBINIT_CONTENT}'")
    endif()
endif()

if (EXISTS ${LLDBINIT_FILE})
    file(READ ${LLDBINIT_FILE} LLDBINIT_EXISTING_CONTENT)
    if (NOT "${LLDBINIT_EXISTING_CONTENT}" MATCHES "${LLDBINIT_CONTENT}")
        file(APPEND ${LLDBINIT_FILE} "${LLDBINIT_CONTENT}\n")
        message(STATUS "Добавлено 'command script import lldb_printer.py' в .lldbinit")
    else()
        message(STATUS "Строка 'command script import lldb_printer.py' уже присутствует в .lldbinit")
    endif()
else()
    file(WRITE ${LLDBINIT_FILE} "${LLDBINIT_CONTENT}\n")
    message(STATUS "Файл .lldbinit создан с содержимым 'command script import lldb_printer.py'")
endif()

set(GDBINIT_FILE ${CMAKE_SOURCE_DIR}/.gdbinit)
set(GDBINIT_CONTENT "source gdb_printer.py")

if (EXISTS ${GDBINIT_FILE})
    file(READ ${GDBINIT_FILE} GDBINIT_EXISTING_CONTENT)
    if (NOT "${GDBINIT_EXISTING_CONTENT}" MATCHES "${GDBINIT_CONTENT}")
        file(APPEND ${GDBINIT_FILE} "${GDBINIT_CONTENT}\n")
        message(STATUS "Добавлено 'source gdb_printer.py' в .gdbinit")
    else()
        message(STATUS "Строка 'source gdb_printer.py' уже присутствует в .gdbinit")
    endif()
else()
    file(WRITE ${GDBINIT_FILE} "${GDBINIT_CONTENT}\n")
    message(STATUS "Файл .gdbinit создан с содержимым 'source gdb_printer.py'")
endif()

if (UNIX)
    set(HOME_DIR $ENV{HOME})

    add_custom_target(configure_gdbinit ALL
            COMMAND sh -c "echo 'directory ${CMAKE_SOURCE_DIR}' > ${HOME_DIR}/.gdbinit"
            COMMAND sh -c "echo 'add-auto-load-safe-path /' >> ${HOME_DIR}/.gdbinit"
            COMMAND sh -c "echo 'set auto-load local-gdbinit on' >> ${HOME_DIR}/.gdbinit"
            COMMENT "Настройка .gdbinit для отладки"
            VERBATIM
    )

    message(WARNING "Файл ~/.gdbinit будет перезаписан!")
endif()

configure_file(${CMAKE_SOURCE_DIR}/gdb_printer.py ${CMAKE_BINARY_DIR}/gdb_printer.py COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/.gdbinit ${CMAKE_BINARY_DIR}/.gdbinit COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/.lldbinit ${CMAKE_BINARY_DIR}/.lldbinit COPYONLY)

install(FILES ${CMAKE_SOURCE_DIR}/gdb_printer.py DESTINATION share/gdb/python)
install(FILES ${CMAKE_SOURCE_DIR}/.gdbinit DESTINATION share/gdb)
install(FILES ${CMAKE_SOURCE_DIR}/.lldbinit DESTINATION share/lldb)

add_subdirectory(tasks)