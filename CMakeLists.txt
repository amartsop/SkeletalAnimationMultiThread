cmake_minimum_required(VERSION 3.16)
project(SkeletalAnimationMultiThread)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Optimization parameters
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_FLAGS "-Os")

# Libigl linking
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
option(LIBIGL_GLFW "Build target igl::glfw" ON)
option(LIBIGL_IMGUI "Build target igl::imgui" ON)
option(LIBIGL_PNG "Build target igl::png" ON)
option(LIBIGL_USE_STATIC_LIBRARY "Use libIGL as static library" ON)
include(libigl)

# Boost linking
find_package(Boost REQUIRED COMPONENTS filesystem system iostreams)

# Include directories
include_directories(
    /usr/local/include/eigen3
    ./include)

# Source files
set(SOURCES
  ./src/serial_com.cpp
  ./src/hand.cpp
  ./src/exoskeleton.cpp
  ./src/utils.cpp
  ./src/skeletal_animation.cpp
  ./src/animated_hand.cpp
  ./src/euler_rotations.cpp
  ./src/menu_handler.cpp
  )

# Libraries
set(ALL_LIBS ${ARMADILLO_LIBRARIES} ${Boost_LIBRARIES}
  igl::glfw igl::imgui igl::png)

# target
add_executable(main main.cpp ${SOURCES})

target_include_directories(main PRIVATE ${ARMADILLO_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})
    
target_link_libraries(main ${ALL_LIBS})