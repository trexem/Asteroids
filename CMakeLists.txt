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
include_directories(${PROJECT_SOURCE_DIR}/vendored/nlohmann)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
set(SDLTTF_VENDORED ON)

add_subdirectory(vendored/sdl_ttf EXCLUDE_FROM_ALL)
set(SDLIMAGE_AVIF OFF)
set(SDLIMAGE_BMP ON)
set(SDLIMAGE_JPEG OFF)
set(SDLIMAGE_PNG ON)
set(SDLIMAGE_WEBP OFF)
set(SDLIMAGE_VENDORED ON)
add_subdirectory(vendored/sdl_image EXCLUDE_FROM_ALL)

# SDL_mixer (used for playing audio)
set(SDLMIXER_MIDI_NATIVE OFF)     # disable formats we don't use to make the build faster and smaller. Also some of these don't work on all platforms so you'll need to do some experimentation.
set(SDLMIXER_GME OFF)
set(SDLMIXER_WAVPACK OFF)     
set(SDLMIXER_MOD OFF)
set(SDLMIXER_OPUS OFF)
set(SDLMIXER_VENDORED ON)   # tell SDL_mixer to build its own dependencies
add_subdirectory(vendored/sdl_mixer EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} ${SOURCES})
 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set(DATA_SOURCE_DIR "${PROJECT_SOURCE_DIR}/data")
set(DATA_DEBUG_DIR "${CMAKE_BINARY_DIR}/Debug/data")
set(DATA_RELEASE_DIR "${CMAKE_BINARY_DIR}/Release/data")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DATA_SOURCE_DIR}
        ${DATA_RELEASE_DIR}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DATA_SOURCE_DIR}
        ${DATA_DEBUG_DIR}
)

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
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
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
    SDL3_mixer::SDL3_mixer
    SDL3::SDL3
  )
