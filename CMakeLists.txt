cmake_minimum_required(VERSION 3.12)

project(lua-rtmidi)

if(WIN32 AND MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS /permissive- /Zc:__cplusplus)
else()
	add_compile_options(-fvisibility=hidden $<$<COMPILE_LANGUAGE:CXX>:-fvisibility-inlines-hidden>)
endif()

# Strip symbols for release builds
if(CMAKE_BUILD_TYPE STREQUAL Release)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
		set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -s")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
	endif()
endif()

if(WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR MINGW)
    option(USE_LIB_PREFIX "Use UNIX style library prefix" ON)
	if(NOT ${USE_LIB_PREFIX})
		set(CMAKE_STATIC_LIBRARY_PREFIX "")
		set(CMAKE_SHARED_LIBRARY_PREFIX "")
	endif()
endif()

if(WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR MINGW)
    option(USE_MINGW_THREADS "Use mingw-std-threads instead of std::thread" OFF)
    if(USE_MINGW_THREADS)
        add_compile_definitions(USE_MINGW_THREADS)
        set(OPTIONAL_LIBS "${OPTIONAL_LIBS} mingw-std-threads")
    endif()
endif()

if(OPTIONAL_LIBS)
    string(STRIP ${OPTIONAL_LIBS} OPTIONAL_LIBS)
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
target_compile_definitions(lua-rtmidi51 PRIVATE LUA_RTMIDI_DLL LUA_RTMIDI_DLL_EXPORTS)
target_link_libraries(lua-rtmidi51 PRIVATE lua51 rtmidi ${OPTIONAL_LIBS})

add_library(lua-rtmidi52 SHARED ${LUA_RTMIDI_SRCS})
target_compile_definitions(lua-rtmidi52 PRIVATE LUA_RTMIDI_DLL LUA_RTMIDI_DLL_EXPORTS)
target_link_libraries(lua-rtmidi52 PRIVATE lua52 rtmidi ${OPTIONAL_LIBS})

add_library(lua-rtmidi53 SHARED ${LUA_RTMIDI_SRCS})
target_compile_definitions(lua-rtmidi53 PRIVATE LUA_RTMIDI_DLL LUA_RTMIDI_DLL_EXPORTS)
target_link_libraries(lua-rtmidi53 PRIVATE lua53 rtmidi ${OPTIONAL_LIBS})
