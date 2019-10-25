#pragma once

#include <lua.hpp>
#include <RtMidi.h>

constexpr const char MT_RTMIDIIN[] = "luartmidi.rtmidiin";
constexpr const char MT_RTMIDIIN_CLASS[] = "luartmidi.rtmidiin.__class";

extern RtMidiIn &getRtMidiIn(lua_State *L, int index);
extern int RtMidiIn_register(lua_State *L);
extern "C" {
__declspec(dllexport) extern int luaopen_luartmidi_rtmidiin(lua_State *L);
}
