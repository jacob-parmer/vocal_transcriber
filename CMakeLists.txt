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
set(libsndfile_lib_dir "${BINARY_DIR}/src/.libs" CACHE 
	PATH "Path to libsndfile lib" FORCE)

set(libsndfile_inc_dir "${BINARY_DIR}/include" CACHE
	PATH "Path to libsndfile headers" FORCE)

set(LIBSNDFILE_INCLUDE_PATH "${install_dir}/src/project_libsndfile-build/src/.libs/")

# Adds portaudio library
ExternalProject_Add(project_portaudio
	GIT_REPOSITORY        https://github.com/PortAudio/portaudio.git
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

# Creates subdirectory for building project and building tests
add_subdirectory(src)
add_subdirectory(test)



