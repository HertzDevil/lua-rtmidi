#pragma once

#include <lua.hpp>
#include <RtMidi.h>
#include "config.hpp"

constexpr const char MT_CHANNELOUT[] = "luartmidi.channelout";

struct LUA_RTMIDI_LOCAL ChannelOut {
	RtMidiOut *output;
	int index;

	template <typename... Ts>
	void send(Ts... xs) {
		const unsigned char buf[] = {static_cast<unsigned char>(xs)...};
		output->sendMessage(buf, sizeof...(xs));
	}
};

extern LUA_RTMIDI_LOCAL ChannelOut &getChannelOut(lua_State *L, int index);
extern LUA_RTMIDI_LOCAL int ChannelOut_register(lua_State *L);
