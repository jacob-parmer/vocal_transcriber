project(test)

# Allow CMake to run tests 
enable_testing()

# Initialize boost
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

# Grabs every test suite
foreach(testSrc ${TEST_SRCS})
	get_filename_component(testName ${testSrc} NAME_WE)

	add_executable(${testName} ${testSrc})
	
	target_link_libraries(${testName} ${Boost_LIBRARIES} helper_files ${EXTRA_LIBS})

	set_target_properties(${testName} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/test)

	add_test(NAME ${testName}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build/test
		COMMAND ${CMAKE_SOURCE_DIR}/build/test/${testName})

endforeach(testSrc)

