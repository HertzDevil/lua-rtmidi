add_library(rtmidi STATIC
	RtMidi.cpp
)
target_include_directories(rtmidi SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if(WIN32)
	target_compile_definitions(rtmidi PRIVATE __WINDOWS_MM__)
	target_link_libraries(rtmidi PRIVATE winmm)
elseif(APPLE)
	target_compile_definitions(rtmidi PRIVATE __MACOSX_CORE__)
	set_target_properties(rtmidi PROPERTIES POSITION_INDEPENDENT_CODE ON)
	find_library(LIB_CORE_MIDI CoreMIDI)
	find_library(LIB_CORE_AUDIO CoreAudio)
	find_library(LIB_CORE_FOUNDATION CoreFoundation)
	target_link_libraries(rtmidi PRIVATE
		${LIB_CORE_MIDI}
		${LIB_CORE_AUDIO}
		${LIB_CORE_FOUNDATION}
	)
else()
	target_compile_definitions(rtmidi PRIVATE __LINUX_ALSA__)
	set_target_properties(rtmidi PROPERTIES POSITION_INDEPENDENT_CODE ON)
	target_link_libraries(rtmidi PRIVATE asound pthread)
endif()
