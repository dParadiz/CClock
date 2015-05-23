cmake_minimum_required(VERSION 2.8)
project(CClock)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)



add_executable(CClock ${SOURCE_FILES} FrameBuffer.h SDLManager.h SDLFontManager.h)

TARGET_LINK_LIBRARIES(CClock SDL2 SDL2_ttf)
