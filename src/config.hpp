#pragma once

#if defined _WIN32 || defined __CYGWIN__
  #define LUA_RTMIDI_HELPER_DLL_IMPORT __declspec(dllimport)
  #define LUA_RTMIDI_HELPER_DLL_EXPORT __declspec(dllexport)
  #define LUA_RTMIDI_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define LUA_RTMIDI_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define LUA_RTMIDI_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define LUA_RTMIDI_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define LUA_RTMIDI_HELPER_DLL_IMPORT
    #define LUA_RTMIDI_HELPER_DLL_EXPORT
    #define LUA_RTMIDI_HELPER_DLL_LOCAL
  #endif
#endif

#ifdef LUA_RTMIDI_DLL
  #ifdef LUA_RTMIDI_DLL_EXPORTS
    #define LUA_RTMIDI_API LUA_RTMIDI_HELPER_DLL_EXPORT
  #else
    #define LUA_RTMIDI_API LUA_RTMIDI_HELPER_DLL_IMPORT
  #endif
  #define LUA_RTMIDI_LOCAL LUA_RTMIDI_HELPER_DLL_LOCAL
#else
  #define LUA_RTMIDI_API
  #define LUA_RTMIDI_LOCAL
#endif
