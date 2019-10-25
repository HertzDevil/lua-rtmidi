# lua-rtmidi

Lua bindings for [RtMidi](http://www.music.mcgill.ca/~gary/rtmidi/index.html).

[API documentation](./doc/index.md)

# Building

**lua-rtmidi** requires CMake 3.12 or above. Only tested on Windows.

```sh
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --target lua-rtmidi53
```

Targets `lua-rtmidi51` and `lua-rtmidi52` are also available, which build the
bindings for Lua 5.1 and 5.2 instead.

# Copyright

**lua-rtmidi** is licensed under the 2-clause BSD License.
