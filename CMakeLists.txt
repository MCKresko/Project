cmake_minimum_required(VERSION 3.16)
project(RobotTowerDefense)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_executable(RobotTowerDefense ${SOURCES})

# For MSYS2 SFML 3.0.2
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

target_link_libraries(RobotTowerDefense PRIVATE SFML::Graphics SFML::Window SFML::System)
