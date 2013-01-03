project(evote-ballot)

add_library(ballot SHARED ballot.cpp)

set(CMAKE_CXX_FLAGS "-std=c++0x")
target_link_libraries(ballot "yaml-cpp")