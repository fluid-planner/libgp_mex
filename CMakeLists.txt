# Copyright 2018 Toyota Research Institute.  All rights reserved.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

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

add_subdirectory(libgp)
add_subdirectory(libgp_interface)

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
