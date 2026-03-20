cmake_minimum_required(VERSION 3.16)
project(image-processing VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(SOURCES
    main.cpp
    src/Filters.cpp
    src/Image.cpp
    src/Photo.cpp
    src/UI.cpp
    src/Utilities.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    src
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    STB_IMAGE_IMPLEMENTATION
    STB_IMAGE_WRITE_IMPLEMENTATION
)

target_link_libraries(${PROJECT_NAME} PRIVATE m)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)