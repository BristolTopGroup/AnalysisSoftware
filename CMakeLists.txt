# Check if cmake has the required version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)
set(PROJECT_NAME_STR AnalysisSoftware)
PROJECT(${PROJECT_NAME_STR})

if((CMAKE_SYSTEM_PROCESSOR MATCHES "i386") AND (CMAKE_SIZEOF_VOID_P EQUAL 8) AND (APPLE))
	set(CMAKE_OSX_ARCHITECTURES "x86_64")
	MESSAGE(STATUS "Building Bristol Analysis Software for ${CMAKE_OSX_ARCHITECTURES} architecture on ${CMAKE_SYSTEM_NAME}")
else()
	MESSAGE(STATUS "Building Bristol Analysis Software on ${CMAKE_SYSTEM_NAME}")
endif()

# get ROOT
include(FindROOT.cmake)
MESSAGE(STATUS "** ROOT Include path: ${ROOT_INCLUDE_DIR}")
MESSAGE(STATUS "** ROOT Library path: ${ROOT_LIBRARY_DIR}")
MESSAGE(STATUS "** ROOT Libraries: ${ROOT_LIBRARIES}")
include_directories(${ROOT_INCLUDE_DIR})
link_directories(${ROOT_LIBRARY_DIR})

# get boost
# static linking doesn't work on DICE for some reason
if((CMAKE_SYSTEM_PROCESSOR MATCHES "i386") AND (CMAKE_SIZEOF_VOID_P EQUAL 8) AND (APPLE))
        set(Boost_USE_STATIC_LIBS   ON)
else()
        set(Boost_USE_STATIC_LIBS   OFF)
endif()
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS
                filesystem
                system
                python
                program_options
								unit_test_framework
             REQUIRED)
MESSAGE(STATUS "** Boost Include path: ${Boost_INCLUDE_DIR}")
MESSAGE(STATUS "** Boost Library path: ${Boost_LIBRARY_DIRS}")
MESSAGE(STATUS "** Boost Libraries: ${Boost_LIBRARIES}")
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# get python
find_package(PythonLibs REQUIRED)
MESSAGE(STATUS "** Python Include path: ${PYTHON_INCLUDE_DIRS}")
MESSAGE(STATUS "** Python Libraries: ${PYTHON_LIBRARIES}")
include_directories(${PYTHON_INCLUDE_DIRS})

#Check the compiler and set the compile and link flags
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-O3 -std=c++11")
# first attempt to make cmake work again on OS X
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -stdlib=libc++")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -emit-llvm -std=c++11")
endif()
#Adding files to compile
aux_source_directory(bin/ SRC_COMMON)
file(GLOB_RECURSE SRC "src/*.cpp")
# some other checks
include(CheckSTLContainers.cmake)
CHECK_STL_CONTAINERS()
#Excluding unnecessary files
file(GLOB_RECURSE SRC_EXCLUDE "src/*HitFitAnalyser.cpp")
list(REMOVE_ITEM SRC ${SRC_EXCLUDE})
#Adding the executable
add_executable(AnalysisSoftware ${SRC} ${SRC_COMMON})

target_link_libraries(AnalysisSoftware
        ${Boost_LIBRARIES}
        ${PYTHON_LIBRARIES}
        ${ROOT_LIBRARIES}
    )

# tests
enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${COMMON_INCLUDES})
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_test(test1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_TEST_NAME})
