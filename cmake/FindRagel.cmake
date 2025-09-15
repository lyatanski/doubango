find_program(RAGEL_CMD NAMES ragel DOC "Ragel state machine compiler")
mark_as_advanced(RAGEL_CMD)

execute_process(COMMAND ${RAGEL_CMD} --version
    OUTPUT_VARIABLE RAGEL_version_output
    ERROR_VARIABLE  RAGEL_version_error
    RESULT_VARIABLE RAGEL_version_result
    OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX REPLACE "^Ragel State Machine Compiler version ([^ ]+) .*$"
                     "\\1"
                     RAGEL_VER "${RAGEL_version_output}")

macro(RAGEL Target Output Input)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${Output}
        COMMAND ${RAGEL_CMD}
        ARGS    -C -L -T0 -o ${CMAKE_CURRENT_SOURCE_DIR}/${Output} ${Input}
        DEPENDS ${Input}
        COMMENT "[RAGEL] Compiling ${Input} state machine with Ragel"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    list(APPEND RAGEL_${Target}_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${Output})
endmacro()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Ragel REQUIRED_VARS RAGEL_CMD
                                        VERSION_VAR   RAGEL_VER)
