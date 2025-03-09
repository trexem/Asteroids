include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.11.0)

set(NAME asteroids)

message(STATUS "using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH) 
      message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()

project(${NAME} VERSION 0.0.01)

if (DEFINED SDL2_PATH)
  message(STATUS "Using SDL2 path specified in .env")
  set(SDL2_INCLUDE_DIRS "${SDL2_PATH}/include")
  set(SDL2_LIB "${SDL2_PATH}/lib/x64")
else()
  find_package(sdl2 2.0.22 REQUIRED)
  set(SDL2_LIB sdl2)
  message(STATUS "Found SDL2")
endif()
if (NOT SDL2_LIB)
    message(FATAL_ERROR "Could not find sdl2 library!")
else()
    message(STATUS "Using sdl2 lib at: ${SDL2_LIB}")
endif()

if (DEFINED SDL2_IMAGE_PATH)
  message(STATUS "Using SDL2_IMAGE path specified in .env")
  set(SDL2_IMAGE_INCLUDE_DIRS "${SDL2_IMAGE_PATH}/include")
  set(SDL2_IMAGE_LIB "${SDL2_IMAGE_PATH}/lib/x64")
else()
  find_package(sdl2_image 2.0.5 REQUIRED)
  set(SDL2_IMAGE_LIB sdl2)
  message(STATUS "Found SDL2_image")
endif()
if (NOT SDL2_IMAGE_LIB)
    message(FATAL_ERROR "Could not find sdl2_image library!")
else()
    message(STATUS "Using sdl2_image lib at: ${SDL2_IMAGE_LIB}")
endif()

if (DEFINED SDL2_TTF)
  message(STATUS "Using SDL2_TTF path specified in .env")
  set(SDL2_TTF_INCLUDE_DIRS "${SDL2_TTF}/include")
  set(SDL2_TTF_LIB "${SDL2_TTF}/lib/x64")
else()
  find_package(sdl2_ttf 2.0.5 REQUIRED)
  set(SDL2_TTF_LIB sdl2)
  message(STATUS "Found SDL2_ttf")
endif()
if (NOT SDL2_TTF_LIB)
    message(FATAL_ERROR "Could not find sdl2_ttf library!")
else()
    message(STATUS "Using sdl2_ttf lib at: ${SDL2_TTF_LIB}")
endif()
 
include_directories(external)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
 
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/release")
 
if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")
 
  if (USE_MINGW)
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/include
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/lib
    )
  endif()
 
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    )
 
  target_link_directories(${PROJECT_NAME} PUBLIC
    ${SDL2_LIB}
    ${SDL2_IMAGE_LIB}
    ${SDL2_TTF_LIB}
  )
 
  target_link_libraries(${PROJECT_NAME} sdl2 sdl2_image sdl2_ttf)
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${PROJECT_SOURCE_DIR}/src
    )
    target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES})
endif()