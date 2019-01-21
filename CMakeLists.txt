cmake_minimum_required(VERSION 2.8)
project(libgp_mex)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Eigen3 3.0.1 REQUIRED)
find_package(MATLAB REQUIRED)

include(ExternalProject)

# MATLAB Macro definitions
add_definitions(/DMATLAB_MEX_FILE)
add_definitions(/DMX_COMPAT_32)

add_subdirectory(modules/libgp)
add_subdirectory(modules/libgp_interface)

include_directories(include
                    external
                    ${EIGEN3_INCLUDE_DIR}
                    ${MATLAB_INCLUDE_DIR}
                    ${libgp_INCLUDE_DIRS}
                    ${libgp_interface_INCLUDE_DIRS}
)

add_library(libgp_mex SHARED src/libgpmex_interface.cc)

target_Link_libraries(libgp_mex
                      gp
                      libgp_interface
                      ${MATLAB_LIBRARIES}
)

if (UNIX AND APPLE)
  set_target_properties(libgp_mex PROPERTIES SUFFIX .mexmaci64 PREFIX "")
elseif (UNIX)
  set_target_properties(libgp_mex PROPERTIES SUFFIX .mexa64 PREFIX "")
endif()
