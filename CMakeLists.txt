cmake_minimum_required(VERSION 3.11.0)

set(NAME asteroids)

project(${NAME} VERSION 0.0.01)

include_directories(external)

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
 
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf SDL3::SDL3)
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${PROJECT_SOURCE_DIR}/src
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE 
      SDL3_ttf::SDL3_ttf 
      SDL3_image::SDL3_image 
      SDL3::SDL3
    )
endif()

