project(exec)

add_library(libsndfile STATIC IMPORTED)
set_property(TARGET libsndfile PROPERTY
	IMPORTED_LOCATION ${libsndfile_lib_dir}/libsndfile.a)

add_library(portaudio STATIC IMPORTED)
set_property(TARGET portaudio PROPERTY
	IMPORTED_LOCATION "${portaudio_lib_dir}/libportaudio.a")

set(EXTRA_LIBS rt asound jack pthread)

add_library(helper_files STATIC
	Audio.cpp
	AudioUtil.cpp
	AudioFeatures.cpp
	SignalProcessing.cpp
)	

target_include_directories(helper_files
	PUBLIC
 	    ${CMAKE_SOURCE_DIR}/include
)	    

include_directories(include ${libsndfile_inc_dir} ${portaudio_inc_dir})

add_executable(exec
      main.cpp)

set_target_properties(exec
	PROPERTIES
	CXX_STANDARD 17)

target_link_libraries(exec helper_files libsndfile portaudio ${EXTRA_LIBS})

