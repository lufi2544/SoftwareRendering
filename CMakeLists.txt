cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(SoftwareRendering VERSION 1.0 LANGUAGES C)

# Set the C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")  # Generate debug symbols
endif()



# Use file(GLOB ...) to include all .cpp files in the source directory
file(GLOB_RECURSE SOURCES 
 "source/main.c"
 "source/*.h"
)


# Add the executable with all gathered source files
add_executable(SoftwareRendering ${SOURCES})

#Link dirs for static libs
target_link_directories(SoftwareRendering PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

#Link SDL2 libs
target_link_libraries(SoftwareRendering PRIVATE
	SDL2Main
	SDL2
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)


# Optionally add include directories
target_include_directories(SoftwareRendering PRIVATE 
 ${CMAKE_CURRENT_SOURCE_DIR}/source
)

# Print debug information
message(STATUS "Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Compiler Flags (Debug): ${CMAKE_C_FLAGS_DEBUG}")