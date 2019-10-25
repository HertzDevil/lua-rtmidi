#pragma once

#include <lua.hpp>
#include <RtMidi.h>
#include "config.hpp"

constexpr const char MT_RTMIDI[] = "luartmidi.rtmidi";

extern LUA_RTMIDI_LOCAL RtMidi &getRtMidi(lua_State *L, int index);
extern LUA_RTMIDI_LOCAL void RtMidi_init(RtMidi &midi, lua_State *L, int index);
extern LUA_RTMIDI_LOCAL void RtMidi_gc(RtMidi &midi, lua_State *L);
extern LUA_RTMIDI_LOCAL int RtMidi_register(lua_State *L);
extern "C" {
extern LUA_RTMIDI_API int luaopen_luartmidi_rtmidi(lua_State *L);
}
