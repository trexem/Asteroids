cmake_minimum_required(VERSION 3.11.0)

set(NAME asteroids)

project(${NAME} VERSION 0.0.01)

if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH) 
      message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SUBDIRS LIST_DIRECTORIES true ${PROJECT_SOURCE_DIR}/include/*)
foreach(SUBDIR ${SUBDIRS})
    if(IS_DIRECTORY ${SUBDIR})
        include_directories(${SUBDIR})
    endif()
endforeach()

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
set(SDLTTF_VENDORED ON)
add_subdirectory(vendored/sdl_ttf EXCLUDE_FROM_ALL)
add_subdirectory(vendored/sdl_image EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} ${SOURCES})
 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)

if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /std:c++latest)
endif()

if (USE_MINGW)
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${MINGW_PATH}/include
  )
  target_link_directories(${PROJECT_NAME} PUBLIC
    ${MINGW_PATH}/lib
  )
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak")
endif()
 
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/release")
 
  message(STATUS "CREATING BUILD FOR UNIX")
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
  )
  target_link_libraries(${PROJECT_NAME} PRIVATE 
    SDL3_ttf::SDL3_ttf 
    SDL3_image::SDL3_image 
    SDL3::SDL3
  )
