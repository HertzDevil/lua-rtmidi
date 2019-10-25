#pragma once

#include <lua.hpp>
#include <RtMidi.h>

constexpr const char MT_CHANNELOUT[] = "luartmidi.channelout";

struct ChannelOut {
	RtMidiOut *output;
	int index;

	template <typename... Ts>
	void send(Ts... xs) {
		const unsigned char buf[] = {static_cast<unsigned char>(xs)...};
		output->sendMessage(buf, sizeof...(xs));
	}
};

extern ChannelOut &getChannelOut(lua_State *L, int index);
extern int ChannelOut_register(lua_State *L);
