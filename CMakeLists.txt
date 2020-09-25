# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.2.2) 

# Set the project name
project (vocal_transcriber)

INCLUDE(ExternalProject)

# LIBRARIES

# Adds libsndfile library
ExternalProject_Add(project_libsndfile
	GIT_REPOSITORY        https://github.com/erikd/libsndfile.git
	PREFIX                lib/libsndfile
	CONFIGURE_COMMAND     <SOURCE_DIR>/configure
	BUILD_COMMAND         make
	BUILD_IN_SOURCE       1
	INSTALL_COMMAND       echo Skipping install step for libsndfile
)

# Pre-configuration step to setting up libsndfile
ExternalProject_Add_Step(project_libsndfile autogen
	COMMAND               <SOURCE_DIR>/autogen.sh
	DEPENDEES             update
	DEPENDERS             configure
)

ExternalProject_Get_Property(project_libsndfile BINARY_DIR)
set(libsndfile_lib_dir "${BINARY_DIR}/src/.libs")
set(libsndfile_inc_dir "${BINARY_DIR}/src")

add_library(libsndfile STATIC IMPORTED)
set_property(TARGET libsndfile PROPERTY
	IMPORTED_LOCATION ${libsndfile_lib_dir}/libsndfile.a)

set(LIBSNDFILE_INCLUDE_PATH "${install_dir}/src/project_libsndfile-build/src/")

ExternalProject_Add(project_portaudio
	SVN_REPOSITORY        https://subversion.assembla.com/svn/portaudio/portaudio/trunk
	SVN_TRUST_CERT        1
	PREFIX                lib/portaudio
	CONFIGURE_COMMAND     <SOURCE_DIR>/configure
	BUILD_IN_SOURCE       1
	BUILD_COMMAND         make
	INSTALL_COMMAND       echo Skipping install step for portaudio
)

ExternalProject_Get_Property(project_portaudio BINARY_DIR)
ExternalProject_Get_Property(project_portaudio SOURCE_DIR)
set(portaudio_lib_dir "${BINARY_DIR}/lib/.libs")
set(portaudio_inc_dir "${SOURCE_DIR}/include")

add_library(portaudio STATIC IMPORTED)
set_property(TARGET portaudio PROPERTY
	IMPORTED_LOCATION "${portaudio_lib_dir}/libportaudio.a")

if(APPLE)
    find_library(CORE_AUDIO_LIBRARY CoreAudio)
    find_library(AUDIO_TOOLBOX_LIBRARY AudioToolbox)
    find_library(AUDIO_UNIT_LIBRARY AudioUnit)
    find_library(CARBON_LIBRARY Carbon)
    set(EXTRA_LIBS ${CORE_AUDIO_LIBRARY} ${AUDIO_TOOLBOX_LIBRARY} ${AUDIO_UNIT_LIBRARY} ${CARBON_LIBRARY})
endif(APPLE)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(EXTRA_LIBS rt asound jack pthread)
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

add_library(helper_files STATIC
	src/Audio.cpp
	src/AudioUtil.cpp
	src/AudioFeatures.cpp
	src/SignalProcessing.cpp
)

target_include_directories(helper_files
	PUBLIC
	    ${PROJECT_SOURCE_DIR}/include
	    ${PROJECT_SOURCE_DIR}/lib
)

include_directories(include ${libsndfile_inc_dir} ${portaudio_inc_dir})

add_executable(vocal_transcriber
       src/main.cpp
)

set_target_properties(vocal_transcriber
	PROPERTIES
	CXX_STANDARD 17)

target_link_libraries(vocal_transcriber ${EXTRA_LIBS} libsndfile portaudio helper_files)





