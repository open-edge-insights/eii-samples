# Copyright (c) 2021 Intel Corporation.
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

cmake_minimum_required(VERSION 3.15)
project(test-publisher VERSION 2.6.0 LANGUAGES C CXX)

# Set C++ version
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

# Set CFLAGS
set(CMAKE_C_FLAGS "-fPIE -fPIC -O2 -Wall -pedantic -fstack-protector-strong -D_FORTIFY_SOURCE=2")
set(CMAKE_CXX_FLAGS "-fPIE -fPIC -O2 -Wall -pedantic -fstack-protector-strong -D_FORTIFY_SOURCE=2")

# Set LDFLAGS
set(CMAKE_SHARED_LINKER_FLAGS "-z noexecstack -z relro -z now")

# Find packages
find_package(EIIUtils REQUIRED)
find_package(EIIMessageBus REQUIRED)
find_package(EIIConfigMgr REQUIRED)
find_package(IntelSafeString REQUIRED)

# Include header directories
include_directories(
    include/
    ${IntelSafeString_INCLUDE}
    ${EIIUtils_INCLUDE}
    ${EIIMessageBus_INCLUDE}
    ${EIIConfigMgr_INCLUDE})

# Add executable
file(GLOB SOURCES "src/*.cpp")
add_executable(run ${SOURCES})

target_link_libraries(run
    ${IntelSafeString_LIBRARIES}
    ${EIIUtils_LIBRARIES}
    ${EIIConfigMgr_LIBRARIES}
    ${EIIMessageBus_LIBRARIES})
