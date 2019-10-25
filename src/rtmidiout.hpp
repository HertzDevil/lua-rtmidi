#pragma once

#include <lua.hpp>
#include <RtMidi.h>

constexpr const char MT_RTMIDIOUT[] = "luartmidi.rtmidiout";
constexpr const char MT_RTMIDIOUT_CLASS[] = "luartmidi.rtmidiout.__class";

extern RtMidiOut &getRtMidiOut(lua_State *L, int index);
extern int RtMidiOut_register(lua_State *L);
extern "C" {
__declspec(dllexport) extern int luaopen_luartmidi_rtmidiout(lua_State *L);
}
