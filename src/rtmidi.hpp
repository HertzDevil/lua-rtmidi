#pragma once

#include <lua.hpp>
#include <RtMidi.h>

constexpr const char MT_RTMIDI[] = "luartmidi.rtmidi";

extern RtMidi &getRtMidi(lua_State *L, int index);
extern void RtMidi_init(RtMidi &midi, lua_State *L, int index);
extern void RtMidi_gc(RtMidi &midi, lua_State *L);
extern int RtMidi_register(lua_State *L);
extern "C" {
__declspec(dllexport) extern int luaopen_luartmidi_rtmidi(lua_State *L);
}
