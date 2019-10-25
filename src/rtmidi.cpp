#include "rtmidi.hpp"
#include "rtmidierror.hpp"
#include "util.hpp"
#include <map>

namespace {

//unsigned char REGISTRY_CALLBACK_ENABLED = 0;

using RtMidi_callback_rec = std::pair<RtMidi *, lua_State *>;
std::map<RtMidi_callback_rec, int> RtMidi_callbacks;

void RtMidiDefaultErrorCallback(RtMidiError::Type typ, const std::string &errorText, void *userData) {
	auto *L = reinterpret_cast<lua_State *>(userData);

	if (typ == RtMidiError::Type::WARNING) {
		RtMidiError {errorText, typ}.printMessage();
	}
	else if (typ == RtMidiError::Type::DEBUG_WARNING) {
//		RtMidiError {errorText, typ}.printMessage();
	}
	else {
		auto midierror = reinterpret_cast<RtMidiError *>(
		 lua_newuserdata(L, sizeof(RtMidiError)));
		new (midierror) RtMidiError {errorText, typ};
		luaL_getmetatable(L, MT_RTMIDIERROR);
		lua_setmetatable(L, -2);
		lua_error(L);
	}
}

void RtMidiCustomErrorCallback(RtMidiError::Type typ, const std::string &errorText, void *userData) {
	auto *rec = reinterpret_cast<RtMidi_callback_rec *>(userData);
	auto it = RtMidi_callbacks.find(*rec);
	if (it == RtMidi_callbacks.end())
		std::cerr << "RtMidi instance not found!\n";

//	if (it != RtMidi_callbacks.end()) {
		lua_rawgeti(rec->second, LUA_REGISTRYINDEX, it->second); // RtMidi_callbackwrapper
		lua_pushinteger(rec->second, typ);
		lua_pushlstring(rec->second, errorText.c_str(), errorText.size());
		lua_call(rec->second, 2, 0);
//	}
//	else {
//		std::cerr << "Function ref not found, falling back to RtMidiDefaultErrorCallback";
//		return RtMidiDefaultErrorCallback(typ, errorText, rec->second);
//	}
}

int RtMidi_callbackwrapper(lua_State *L) {
	lua_pushvalue(L, lua_upvalueindex(1)); // error callback
	lua_pushvalue(L, 1);
	lua_pushvalue(L, 2);
	lua_pushvalue(L, lua_upvalueindex(2)); // user data
	lua_call(L, 3, 0);
	return 0;
}

int RtMidi_openport(lua_State *L) {
	auto &self = getRtMidi(L, 1);
	self.openPort(static_cast<unsigned>(luaL_optinteger(L, 2, 1)) - 1);
	return 0;
}

int RtMidi_getportcount(lua_State *L) {
	auto &self = getRtMidi(L, 1);
	lua_pushinteger(L, self.getPortCount());
	return 1;
}

int RtMidi_getportname(lua_State *L) {
	auto &self = getRtMidi(L, 1);
	auto name = self.getPortName(static_cast<unsigned>(luaL_optinteger(L, 2, 1)) - 1);
	lua_pushlstring(L, name.c_str(), name.size());
	return 1;
}

int RtMidi_closeport(lua_State *L) {
	auto &self = getRtMidi(L, 1);
	self.closePort();
	return 0;
}

int RtMidi_isportopen(lua_State *L) {
	auto &self = getRtMidi(L, 1);
	lua_pushboolean(L, self.isPortOpen());
	return 1;
}

int RtMidi_seterrorcallback(lua_State *L) {
	auto &self = getRtMidi(L, 1);
	if (lua_isnoneornil(L, 2))
		RtMidi_init(self, L, 1);
	else {
		lua_pushvalue(L, 2);
		lua_pushvalue(L, 3);
		lua_pushcclosure(L, RtMidi_callbackwrapper, 2);
		RtMidi_callbacks[{&self, L}] = luaL_ref(L, LUA_REGISTRYINDEX);
		auto it = RtMidi_callbacks.find({&self, L});
		self.setErrorCallback(RtMidiCustomErrorCallback,
		 static_cast<void *>(const_cast<RtMidi_callback_rec *>(&it->first)));

//		lua_pushlightuserdata(L, &REGISTRY_CALLBACK_ENABLED);
//		lua_rawget(L, LUA_REGISTRYINDEX);
//		lua_pushvalue(L, 1);
//		lua_pushboolean(L, true);
//		lua_rawset(L, -3);
//		lua_pop(L, 1);
	}
	return 0;
}

#define METHOD(X) {#X, RtMidi_##X}
const luaL_Reg RtMidi_mt_index[] = {
	// virtual method wrappers
	METHOD(openport),
//	METHOD(openvirtualport),
	METHOD(getportcount),
	METHOD(getportname),
	METHOD(closeport),
	METHOD(isportopen),
	METHOD(seterrorcallback),

	{nullptr, nullptr},
};
#undef METHOD

int RtMidi_getversion(lua_State *L) {
	auto str = RtMidi::getVersion();
	lua_pushlstring(L, str.c_str(), str.size());
	return 1;
}

int RtMidi_compiledapis(lua_State *L) {
	auto apis = std::vector<RtMidi::Api> { };
	RtMidi::getCompiledApi(apis);
	lua_createtable(L, apis.size(), 0);
	for (std::size_t i = 0; i < apis.size(); ++i) {
		lua_pushinteger(L, apis[i]);
		lua_rawseti(L, -2, i + 1);
	}
	return 1;
}

int RtMidi_getapiname(lua_State *L) {
	auto api = static_cast<RtMidi::Api>(luaL_checkinteger(L, 1));
	auto str = RtMidi::getApiName(api);
	lua_pushlstring(L, str.c_str(), str.size());
	return 1;
}

int RtMidi_getapidisplayname(lua_State *L) {
	auto api = static_cast<RtMidi::Api>(luaL_checkinteger(L, 1));
	auto str = RtMidi::getApiDisplayName(api);
	lua_pushlstring(L, str.c_str(), str.size());
	return 1;
}

int RtMidi_getcompiledapibyname(lua_State *L) {
	std::size_t sz;
	const char *str = luaL_checklstring(L, 1, &sz);
	lua_pushinteger(L, RtMidi::getCompiledApiByName({str, sz}));
	return 1;
}

#define METHOD(X) {#X, RtMidi_##X}
const luaL_Reg RtMidi_static_methods[] = {
	METHOD(getversion),
	METHOD(compiledapis),
	METHOD(getapiname),
	METHOD(getapidisplayname),
	METHOD(getcompiledapibyname),

	{nullptr, nullptr},
};
#undef METHOD

#define ENUM(X) {RtMidi::Api::X, #X}
const std::pair<RtMidi::Api, const char *> RTMIDI_API_VALUES[] = {
	ENUM(UNSPECIFIED),
	ENUM(MACOSX_CORE),
	ENUM(LINUX_ALSA),
	ENUM(UNIX_JACK),
	ENUM(WINDOWS_MM),
	ENUM(RTMIDI_DUMMY),
	ENUM(NUM_APIS),
};
#undef ENUM

} // namespace



RtMidi &getRtMidi(lua_State *L, int index) {
	luaL_getmetatable(L, MT_RTMIDI);
	return luaGetUserdata<RtMidi>(L, index, "RtMidi");
}

void RtMidi_init(RtMidi &midi, lua_State *L, int index) {
	midi.setErrorCallback(RtMidiDefaultErrorCallback, L);
	RtMidi_callbacks[{&midi, L}] = LUA_NOREF;

//	lua_pushlightuserdata(L, &REGISTRY_CALLBACK_ENABLED);
//	lua_rawget(L, LUA_REGISTRYINDEX);
//	lua_pushvalue(L, index);
//	lua_pushnil(L);
//	lua_rawset(L, -3);
//	lua_pop(L, 1);
}

void RtMidi_gc(RtMidi &midi, lua_State *L) {
	auto it = RtMidi_callbacks.find({&midi, L});
	if (it != RtMidi_callbacks.end()) {
		luaL_unref(L, LUA_REGISTRYINDEX, it->second);
		RtMidi_callbacks.erase(it);
	}
}

int RtMidi_register(lua_State *L) {
//	lua_pushlightuserdata(L, &REGISTRY_CALLBACK_ENABLED);
//	lua_rawget(L, LUA_REGISTRYINDEX);
//	if (!lua_istable(L, -1)) {
//		lua_pop(L, 1);
//		lua_newtable(L);
//		lua_createtable(L, 0, 1);
//		lua_pushliteral(L, "__mode");
//		lua_pushliteral(L, "k");
//		lua_rawset(L, -3);
//		lua_setmetatable(L, -2);
//		lua_pushlightuserdata(L, &REGISTRY_CALLBACK_ENABLED);
//		lua_pushvalue(L, -2);
//		lua_rawset(L, LUA_REGISTRYINDEX);
//	}
//	lua_pop(L, 1);

	if (luaL_newmetatable(L, MT_RTMIDI)) {
		lua_pushliteral(L, "__index");
		luaNewLib(L, RtMidi_mt_index);
		lua_rawset(L, -3);
	}
	lua_pop(L, 1);

	luaNewLib(L, RtMidi_static_methods);

	// enum
	lua_pushliteral(L, "Api");
	lua_createtable(L, ARRAY_SIZE(RTMIDI_API_VALUES), ARRAY_SIZE(RTMIDI_API_VALUES));
	for (const auto &p : RTMIDI_API_VALUES) {
		lua_pushstring(L, p.second);
		lua_rawseti(L, -2, p.first);
		lua_pushstring(L, p.second);
		lua_pushinteger(L, p.first);
		lua_rawset(L, -3);
	}
	lua_rawset(L, -3);

	return 1;
}

extern "C" {

__declspec(dllexport) int luaopen_luartmidi_rtmidi(lua_State *L) {
	return RtMidi_register(L);
}

}
