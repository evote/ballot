project(evote-ballot)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++0x")

add_library(ballot SHARED ballot.cpp)
target_link_libraries(ballot "yaml-cpp")
target_link_libraries(ballot "gmp")

add_library(pyballot MODULE pyballot.cpp)
target_link_libraries(pyballot ballot)
target_link_libraries(pyballot boost_python)
set_target_properties(pyballot PROPERTIES
	PREFIX ""
	INSTALL_RPATH "$ORIGIN"
)

find_package( Boost 1.35 REQUIRED COMPONENTS python )
include_directories(${Boost_INCLUDE_DIRS})
find_package( PythonLibs 2.7 REQUIRED )
include_directories(${PYTHON_INCLUDE_PATH})

install(TARGETS ballot pyballot DESTINATION .)
install(PROGRAMS ballot.cgi DESTINATION .)
