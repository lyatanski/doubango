cmake_minimum_required(VERSION 3.16)
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${name})

find_package(SWIG 4.0 COMPONENTS python java)
find_package(Python COMPONENTS Development Interpreter)
find_package(JNI)

file(GLOB src _common/*.cxx)
add_library(wrap ${src})
target_include_directories(wrap PUBLIC _common)
target_link_libraries(wrap tinySIP tinyDAV tinySMS tinyXCAP)

if(SWIG_FOUND)
    include(${SWIG_USE_FILE})
    set_property(SOURCE _common/tinyWRAP.i PROPERTY CPLUSPLUS ON)

        #swig_add_library(goWRAP LANGUAGE go OUTPUT_DIR go SOURCES _common/tinyWRAP.i)
        #set_target_properties(goWRAP PROPERTIES SWIG_COMPILE_OPTIONS "-intgosize;64")
        #set_target_properties(goWRAP PROPERTIES LIBRARY_OUTPUT_DIRECTORY go)
        #target_link_libraries(goWRAP wrap)

    if(Python_Development_FOUND)
        swig_add_library(pyWRAP LANGUAGE python OUTPUT_DIR python SOURCES _common/tinyWRAP.i)
        target_include_directories(pyWRAP PUBLIC ${Python_INCLUDE_DIRS})
        set_target_properties(pyWRAP PROPERTIES LIBRARY_OUTPUT_DIRECTORY python)
        target_link_libraries(pyWRAP wrap)
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/python/setup.py [[
from setuptools import setup
setup(name='tinyWRAP',
    version='1.0',
    py_modules=['tinyWRAP'],
    packages=[''],
    package_dir={'': '.'},
    package_data={'': ['_pyWRAP.so']},
)
]])
    install(CODE "execute_process(COMMAND ${Python_EXECUTABLE} -m pip install ${CMAKE_CURRENT_BINARY_DIR}/python)")
    endif()

    if(JNI_FOUND)
        swig_add_library(jWRAP LANGUAGE java SOURCES _common/tinyWRAP.i OUTPUT_DIR java)
        target_include_directories(jWRAP PUBLIC ${JNI_INCLUDE_DIRS})
        set_target_properties(jWRAP PROPERTIES LIBRARY_OUTPUT_DIRECTORY java)
        target_link_libraries(jWRAP wrap)
    endif()
endif()
