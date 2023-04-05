cmake_minimum_required (VERSION 3.16.3)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

project (<<<NAME>>> VERSION <<<VERSION>>> LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

find_package(microservice-essentials REQUIRED)

#<<<SAPPER SECTION BEGIN CMAKE-SUBDIRECTORIES>>>
add_subdirectory (src)
#<<<SAPPER SECTION END CMAKE-SUBDIRECTORIES>>>
