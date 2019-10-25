#include "rtmidiout.hpp"
#include "channelout.hpp"
#include "rtmidi.hpp"
#include "rtmidierror.hpp"
#include "util.hpp"

namespace {

unsigned char REGISTRY_CHANNELOUT = 0;

int RtMidiOut_ctor(lua_State *L) {
	auto midiout = reinterpret_cast<RtMidiOut *>(
	 lua_newuserdata(L, sizeof(RtMidiOut)));
	try {
		new (midiout) RtMidiOut { };
		RtMidi_init(*midiout, L, -1);
		luaL_getmetatable(L, MT_RTMIDIOUT);
		lua_setmetatable(L, -2);
		return 1;
	}
	catch (RtMidiError &err) {
		auto midierror = reinterpret_cast<RtMidiError *>(
		 lua_newuserdata(L, sizeof(RtMidiError)));
		new (midierror) RtMidiError {err};
		luaL_getmetatable(L, MT_RTMIDIERROR);
		lua_setmetatable(L, -2);
		return lua_error(L);
	}
}

int RtMidiOut_dtor(lua_State *L) {
	auto midiout = reinterpret_cast<RtMidiOut *>(
	 lua_touserdata(L, 1));
	if (midiout) {
		RtMidi_gc(*midiout, L);
		midiout->~RtMidiOut();
	}
	return 0;
}

int RtMidiOut_getcurrentapi(lua_State *L) {
	auto &self = getRtMidiOut(L, 1);
	lua_pushinteger(L, self.getCurrentApi());
	return 1;
}

int RtMidiOut_sendmessage(lua_State *L) {
	auto &self = getRtMidiOut(L, 1);
	int byteCount = lua_gettop(L) - 1;
	auto msg = std::vector<unsigned char>(byteCount);
	for (int i = 0; i < byteCount; ++i)
		msg[i] = static_cast<unsigned char>(luaL_checkinteger(L, i + 2));
	if (msg.empty())
		self.sendMessage(nullptr, 0u);
	else
		self.sendMessage(&msg);
	lua_settop(L, 1);
	return 1;
}

int RtMidiOut_channel(lua_State *L) {
	auto &self = getRtMidiOut(L, 1);
	lua_Integer index = luaL_checkinteger(L, 2);
	luaL_argcheck(L, index >= 0 && index <= 15, 2,
		"MIDI Channel number must be between 0 and 15");

	auto channel = reinterpret_cast<ChannelOut *>(
	 lua_newuserdata(L, sizeof(ChannelOut)));
	new (channel) ChannelOut {&self, static_cast<int>(index)};
	luaL_getmetatable(L, MT_CHANNELOUT);
	lua_setmetatable(L, -2);

	lua_pushlightuserdata(L, &REGISTRY_CHANNELOUT);
	lua_rawget(L, LUA_REGISTRYINDEX);
	lua_pushvalue(L, -2);
	lua_pushvalue(L, 1);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	return 1;
}

int RtMidiOut_sysex(lua_State *L) {
	auto &self = getRtMidiOut(L, 1);
	auto msg = std::vector<unsigned char> { };
	msg.push_back(0xF0u);

	for (int i = 2, n = lua_gettop(L); i <= n; ++i) {
		int typ = lua_type(L, i);
		if (typ == LUA_TSTRING) {
			std::size_t sz;
			const char *buf = lua_tolstring(L, 2, &sz);
			for (std::size_t j = 0; j < sz; ++j) {
				auto b = static_cast<unsigned char>(buf[j]);
				luaL_argcheck(L, b <= 0x7Fu, i, "SysEx message byte must be between 0 and 127");
				msg.push_back(b);
			}
		}
		else if (typ == LUA_TNUMBER) {
			lua_Integer b = luaL_checkinteger(L, i + 2);
			luaL_argcheck(L, b >= 0 && b <= 0x7F, i, "SysEx message byte must be between 0 and 127");
			msg.push_back(static_cast<unsigned char>(b));
		}
		else
			return luaL_typerror(L, i, "number or string");
	}

	msg.push_back(0xF7u);
	self.sendMessage(&msg);
	lua_settop(L, 1);
	return 1;
}

#define METHOD(X) {#X, RtMidiOut_##X}
const luaL_Reg RtMidiOut_mt_index[] = {
	// finalizer
	{"_finalize", RtMidiOut_dtor},

	// method wrappers
	METHOD(getcurrentapi),
	METHOD(sendmessage),

	// extensions
	METHOD(channel),
	METHOD(sysex),

	{nullptr, nullptr},
};
#undef METHOD

} // namespace

LUA_RTMIDI_LOCAL RtMidiOut &getRtMidiOut(lua_State *L, int index) {
	luaL_getmetatable(L, MT_RTMIDIOUT);
	return luaGetUserdata<RtMidiOut>(L, index, "RtMidiOut");
}

LUA_RTMIDI_LOCAL int RtMidiOut_register(lua_State *L) {
	// reference counting for channel objects:
	// each weakly counted ChannelOut object stores a strong ref to RtMidiOut
	lua_pushlightuserdata(L, &REGISTRY_CHANNELOUT);
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
		lua_createtable(L, 0, 1);
		lua_pushliteral(L, "__mode");
		lua_pushliteral(L, "k");
		lua_rawset(L, -3);
		lua_setmetatable(L, -2);
		lua_pushlightuserdata(L, &REGISTRY_CHANNELOUT);
		lua_pushvalue(L, -2);
		lua_rawset(L, LUA_REGISTRYINDEX);
	}
	lua_pop(L, 1);

	// instance methods
	if (luaL_newmetatable(L, MT_RTMIDIOUT)) {
		lua_pushliteral(L, "__index");
		luaNewLib(L, RtMidiOut_mt_index);
		luaL_getmetatable(L, MT_RTMIDI);
		lua_setmetatable(L, -2);
		lua_rawset(L, -3);

		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, RtMidiOut_dtor);
		lua_rawset(L, -3);
	}
	lua_pop(L, 1);

	// constructor
	lua_newtable(L);
	if (luaL_newmetatable(L, MT_RTMIDIOUT_CLASS)) {
		lua_pushliteral(L, "__call");
		lua_pushcfunction(L, RtMidiOut_ctor);
		lua_rawset(L, -3);
	}
	lua_setmetatable(L, -2);

	return 1;
}

extern "C" LUA_RTMIDI_API int luaopen_luartmidi_rtmidiout(lua_State *L) {
	lua_pop(L, ChannelOut_register(L));
	lua_pop(L, RtMidi_register(L));
	return RtMidiOut_register(L);
}
