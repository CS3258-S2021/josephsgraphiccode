cmake_minimum_required(VERSION 3.17)
project(CS3258_cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DGL_SILENCE_DEPRECATION")

set(SOURCE_FILES main.cpp Command.cpp Command.h EndianChecks.cpp EndianChecks.h TiffImageData.cpp TiffImageData.h TiffStatCommand.cpp TiffStatCommand.h TiffReadCommand.cpp TiffReadCommand.h CLI.cpp CLI.h TiffWriteCommand.cpp TiffWriteCommand.h Global.h Tokenizer.cpp Tokenizer.h ResizeCommand.cpp ResizeCommand.h Filter.cpp Filter.h Matrix44.cpp Matrix44.h Vector4.cpp Vector4.h PushCommand.cpp PushCommand.h PopCommand.cpp PopCommand.h TranslateCommand.cpp TranslateCommand.h ScaleCommand.cpp ScaleCommand.h RotateCommand.cpp RotateCommand.h Vector3.cpp Vector3.h OrthoCommand.cpp OrthoCommand.h PerspectiveCommand.cpp PerspectiveCommand.h LookatCommand.cpp LookatCommand.h VertexCommand.cpp VertexCommand.h ResetCommand.cpp ResetCommand.h lines.cpp ScreenCommand.cpp ScreenCommand.h OrthocameraCommand.cpp OrthocameraCommand.h Ray.cpp Ray.h Camera.cpp Camera.h Box.cpp Box.h Shape.cpp Shape.h Sphere.cpp Sphere.h Triangle.cpp Triangle.h Light.cpp Light.h ClearCommand.cpp ClearCommand.h TraceCommand.cpp TraceCommand.h PerspCamera.cpp PerspCamera.h)
add_executable(CS3258_cmake ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
target_link_libraries(CS3258_cmake ${OPENGL_LIBRARIES} ${GLUT_LIBRARY}) # Link libraries.