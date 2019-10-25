#pragma once

#include <lua.hpp>
#include <RtMidi.h>
#include "config.hpp"

constexpr const char MT_RTMIDIOUT[] = "luartmidi.rtmidiout";
constexpr const char MT_RTMIDIOUT_CLASS[] = "luartmidi.rtmidiout.__class";

extern LUA_RTMIDI_LOCAL RtMidiOut &getRtMidiOut(lua_State *L, int index);
extern LUA_RTMIDI_LOCAL int RtMidiOut_register(lua_State *L);
extern "C" {
extern LUA_RTMIDI_API int luaopen_luartmidi_rtmidiout(lua_State *L);
}
