cmake_minimum_required(VERSION 3.17)
project(CS3258_cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DGL_SILENCE_DEPRECATION")

set(SOURCE_FILES main.cpp Command.cpp Command.h EndianChecks.cpp EndianChecks.h TiffImageData.cpp TiffImageData.h TiffStatCommand.cpp TiffStatCommand.h TiffReadCommand.cpp TiffReadCommand.h CLI.cpp CLI.h TiffWriteCommand.cpp TiffWriteCommand.h Global.h Tokenizer.cpp Tokenizer.h)
add_executable(CS3258_cmake ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
target_link_libraries(CS3258_cmake ${OPENGL_LIBRARIES} ${GLUT_LIBRARY}) # Link libraries.