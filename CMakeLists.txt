cmake_minimum_required (VERSION 2.6)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# Some lib stuff, probably unneeded, but I don't know cmake too well yet...
if (WIN32)
	set(LIB_PREFIX )
	set(LIB_POSTFIX .lib)
else ()
	set(LIB_PREFIX lib)
	set(LIB_POSTFIX .so)
endif ()

# Some options:
set(INSTALL_RUNTIMES_TO "/usr/bin" CACHE STRING "Where install executables.")
set(INSTALL_LIBS_TO "/usr/lib" CACHE STRING "Where to install libs.")

# common includes
include_directories("src")

# add the subsystem itself
add_subdirectory(src)
