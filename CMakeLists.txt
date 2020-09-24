# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5) 

# Set the project name
project (vocal_transcriber)

# LIBRARIES

# Generates a static library from the library sources
add_library(helper_files STATIC
	src/Audio.cpp
	src/AudioUtil.cpp
	src/AudioFeatures.cpp
	src/SignalProcessing.cpp
)

target_include_directories(helper_files
	PUBLIC
	    ${PROJECT_SOURCE_DIR}/include
)

find_package(Boost 1.46.1)

if (libsndfile_FOUND)
	message ("portaudio found")
else()
	message (FATAL_ERROR "Cannot find Boost")
endif()


# EXECUTABLE

# Add an executable with main
add_executable (vocal_transcriber 
	src/main.cpp
)

# Links our libraries to the binary target
target_link_libraries(vocal_transcriber 
	PRIVATE
	    helper_files
)

