project(evote-ballot)

add_library(ballot SHARED ballot.cpp)
target_link_libraries(ballot "yaml-cpp")


add_library(pyballot  MODULE pyballot.cpp)
target_link_libraries(pyballot ballot)
target_link_libraries(pyballot boost_python)
set_target_properties(pyballot PROPERTIES
	PREFIX ""
)


find_package( Boost 1.35 REQUIRED COMPONENTS regex python )
include_directories(${Boost_INCLUDE_DIRS})
find_package( PythonLibs 2.7 REQUIRED )
include_directories(${PYTHON_INCLUDE_PATH})


set(CMAKE_CXX_FLAGS "-std=c++0x")
