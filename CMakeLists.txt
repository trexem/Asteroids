cmake_minimum_required(VERSION 3.11.0)

set(NAME asteroids)

project(${NAME} VERSION 0.0.01)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if (ANDROID)
    message(STATUS "Configuring for Android build")
    set(IS_ANDROID TRUE)
else()
    set(IS_ANDROID FALSE)
endif()

enable_language(C) #For miniz library

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

add_library(miniz STATIC ${PROJECT_SOURCE_DIR}/vendored/miniz/miniz.c)

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

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

if (IS_ANDROID)
  function(add_executable TARGET)
    add_library(${TARGET} SHARED ${ARGN})
  endfunction()
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

if (IS_ANDROID)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${PROJECT_SOURCE_DIR}/data.bin
            ${PROJECT_SOURCE_DIR}/android/app/src/main/assets/data.bin
        COMMENT "Copying data.bin to android assets"
    )
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set(DATA_SOURCE_DIR "${PROJECT_SOURCE_DIR}/data")
set(DATA_OUTPUT_DIR "${CMAKE_BINARY_DIR}/$<CONFIG>/data")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DATA_SOURCE_DIR}
        ${DATA_OUTPUT_DIR}
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/obfuscateAssets.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Packing and obfuscating assets into data.bin"
)

if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /std:c++latest)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${PROJECT_SOURCE_DIR}/data.bin
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/data.bin
        COMMENT "Copying data.bin to output directory"
    )
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${PROJECT_SOURCE_DIR}/.env
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/.env
        COMMENT "Copying .env to output directory"
    )
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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/$<CONFIG>")
 
target_include_directories(${PROJECT_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/vendored/miniz
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
  SDL3_ttf::SDL3_ttf 
  SDL3_image::SDL3_image
  SDL3_mixer::SDL3_mixer
  SDL3::SDL3
  miniz
)
