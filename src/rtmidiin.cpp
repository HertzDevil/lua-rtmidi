#include "rtmidiin.hpp"
#include "rtmidi.hpp"
#include "rtmidierror.hpp"
#include "util.hpp"
#include <map>

namespace {

//unsigned char REGISTRY_CALLBACK_ENABLED = 0;

using RtMidiIn_callback_rec = std::pair<RtMidiIn *, lua_State *>;
std::map<RtMidiIn_callback_rec, int> RtMidiIn_callbacks;

void RtMidiInCustomCallback(double timeStamp, std::vector<unsigned char> *message, void *userData) {
	auto *rec = reinterpret_cast<RtMidiIn_callback_rec *>(userData);
	auto it = RtMidiIn_callbacks.find(*rec);
	if (it == RtMidiIn_callbacks.end())
		std::cerr << "RtMidiIn instance not found!\n";
	auto n = message->size();

	lua_rawgeti(rec->second, LUA_REGISTRYINDEX, it->second); // RtMidiIn_callbackwrapper
	lua_pushnumber(rec->second, timeStamp);
	lua_createtable(rec->second, n, 0);
	for (std::size_t i = 0; i < n; ++i) {
		lua_pushinteger(rec->second, (*message)[i]);
		lua_rawseti(rec->second, -2, i + 1);
	}
	lua_call(rec->second, 2, 0);
}

int RtMidiIn_callbackwrapper(lua_State *L) {
	lua_pushvalue(L, lua_upvalueindex(1)); // callback
	lua_pushvalue(L, 1);
	lua_pushvalue(L, 2);
	lua_pushvalue(L, lua_upvalueindex(2)); // user data
	lua_call(L, 3, 0);
	return 0;
}

int RtMidiIn_ctor(lua_State *L) {
	auto midiin = reinterpret_cast<RtMidiIn *>(
	 lua_newuserdata(L, sizeof(RtMidiIn)));
	try {
		new (midiin) RtMidiIn { };
		RtMidi_init(*midiin, L, -1);
		RtMidiIn_callbacks[{midiin, L}] = LUA_NOREF;
		luaL_getmetatable(L, MT_RTMIDIIN);
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

int RtMidiIn_dtor(lua_State *L) {
	auto midiin = reinterpret_cast<RtMidiIn *>(lua_touserdata(L, 1));
	if (midiin) {
		auto it = RtMidiIn_callbacks.find({midiin, L});
		if (it != RtMidiIn_callbacks.end()) {
			luaL_unref(L, LUA_REGISTRYINDEX, it->second);
			RtMidiIn_callbacks.erase(it);
		}
		midiin->~RtMidiIn();
	}
	return 0;
}

int RtMidiIn_getcurrentapi(lua_State *L) {
	auto &self = getRtMidiIn(L, 1);
	lua_pushinteger(L, self.getCurrentApi());
	return 1;
}

int RtMidiIn_setcallback(lua_State *L) {
	auto &self = getRtMidiIn(L, 1);
	if (lua_isnoneornil(L, 2))
		self.cancelCallback();
	else {
		lua_pushvalue(L, 2);
		lua_pushvalue(L, 3);
		lua_pushcclosure(L, RtMidiIn_callbackwrapper, 2);
		RtMidiIn_callbacks[{&self, L}] = luaL_ref(L, LUA_REGISTRYINDEX);
		auto it = RtMidiIn_callbacks.find({&self, L});
		self.setCallback(RtMidiInCustomCallback,
		 static_cast<void *>(const_cast<RtMidiIn_callback_rec *>(&it->first)));
	}
	return 0;
}

int RtMidiIn_ignoretypes(lua_State *L) {
	auto &self = getRtMidiIn(L, 1);
	bool midiSysex = lua_toboolean(L, 2);
	bool midiTime = lua_toboolean(L, 3);
	bool midiSense = lua_toboolean(L, 4);
	self.ignoreTypes(midiSysex, midiTime, midiSense);
	return 0;
}

int RtMidiIn_getmessage(lua_State *L) {
	auto &self = getRtMidiIn(L, 1);
	auto msg = std::vector<unsigned char> { };
	double dt = self.getMessage(&msg);
	auto n = msg.size();

	lua_pushnumber(L, dt);
	lua_createtable(L, n, 0);
	for (std::size_t i = 0; i < n; ++i) {
		lua_pushinteger(L, msg[i]);
		lua_rawseti(L, -2, i + 1);
	}
	return 2;
}

#define METHOD(X) {#X, RtMidiIn_##X}
const luaL_Reg RtMidiIn_mt_index[] = {
	// finalizer
	{"_finalize", RtMidiIn_dtor},

	// method wrappers
	METHOD(getcurrentapi),
	METHOD(setcallback),
//	METHOD(cancelcallback),
	METHOD(ignoretypes),
	METHOD(getmessage),

	{nullptr, nullptr},
};
#undef METHOD

} // namespace

LUA_RTMIDI_LOCAL RtMidiIn &getRtMidiIn(lua_State *L, int index) {
	luaL_getmetatable(L, MT_RTMIDIIN);
	return luaGetUserdata<RtMidiIn>(L, index, "RtMidiIn");
}

LUA_RTMIDI_LOCAL int RtMidiIn_register(lua_State *L) {
	// instance methods
	if (luaL_newmetatable(L, MT_RTMIDIIN)) {
		lua_pushliteral(L, "__index");
		luaNewLib(L, RtMidiIn_mt_index);
		luaL_getmetatable(L, MT_RTMIDI);
		lua_setmetatable(L, -2);
		lua_rawset(L, -3);

		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, RtMidiIn_dtor);
		lua_rawset(L, -3);
	}
	lua_pop(L, 1);

	// constructor
	lua_newtable(L);
	if (luaL_newmetatable(L, MT_RTMIDIIN_CLASS)) {
		lua_pushliteral(L, "__call");
		lua_pushcfunction(L, RtMidiIn_ctor);
		lua_rawset(L, -3);
	}
	lua_setmetatable(L, -2);

	return 1;
}

extern "C" LUA_RTMIDI_API int luaopen_luartmidi_rtmidiin(lua_State *L) {
	lua_pop(L, RtMidi_register(L));
	return RtMidiIn_register(L);
}
