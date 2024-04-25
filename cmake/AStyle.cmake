# Find the program
find_program(ASTYLE astyle DOC "Code Formatter")

# Add a custom target
add_custom_target("format"
    COMMAND "${ASTYLE}"
        --style=k/r
        --lineend=linux
        --mode=c
        --add-brackets
        --break-closing-brackets
        --recursive
        --suffix=none
        --convert-tabs
        *.h *.c *.cxx
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    VERBATIM
)
