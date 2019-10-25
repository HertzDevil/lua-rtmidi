cmake_minimum_required(VERSION 3.12)

project(lua-rtmidi)

if(WIN32)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(ext)

set(LUA_RTMIDI_SRCS
	src/luartmidi.cpp
	src/rtmidi.cpp
	src/rtmidiin.cpp
	src/rtmidiout.cpp
	src/rtmidierror.cpp
	src/channelout.cpp
)

add_library(lua-rtmidi51 SHARED ${LUA_RTMIDI_SRCS})
target_link_libraries(lua-rtmidi51 PRIVATE lua51 rtmidi)

add_library(lua-rtmidi52 SHARED ${LUA_RTMIDI_SRCS})
target_link_libraries(lua-rtmidi52 PRIVATE lua52 rtmidi)

add_library(lua-rtmidi53 SHARED ${LUA_RTMIDI_SRCS})
target_link_libraries(lua-rtmidi53 PRIVATE lua53 rtmidi)
