cmake_minimum_required(VERSION 3.0)
project("GLSaver")


# GLSaver
find_package(OpenGL REQUIRED)

include_directories("GLSaver")
include_directories( ${OPENGL_INCLUDE_DIRS} )

file(GLOB_RECURSE "SOURCE_FILES" "GLSaver/*.c" "GLSaver/*.cpp")

add_executable("GLSaver" WIN32 MACOSX_BUNDLE ${SOURCE_FILES})
target_link_libraries("GLSaver" ${OPENGL_LIBRARIES})


