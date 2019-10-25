# luartmidi.rtmidi

Base class for MIDI input and output.

## Enumeration types

### Api

```lua
{
  UNSPECIFIED = 0,
  MACOSX_CORE = 1,
  LINUX_ALSA = 2,
  UNIX_JACK = 3,
  WINDOWS_MM = 4,
  RTMIDI_DUMMY = 5,
  NUM_APIS = 6,
}
```

Arguments for the constructors of [`RtMidiIn`](./luartmidi.rtmidiin.md) and
[`RtMidiOut`](./luartmidi.rtmidiout.md) classes to indicate which API the MIDI
object should use.

## Module functions

### getversion ()

Returns the version string of the RtMidi library.

### compiledapis ()

Returns a sequence containing `RtMidi.Api` values for the APIs supported by the
RtMidi library.

### getapiname (api)

Returns the short name of the API with the specified `RtMidi.Api` value.

### getapidisplayname (api)

Returns a descriptive name of the API with the specified `RtMidi.Api` value.

### getcompiledapibyname (name)

Returns the `RtMidi.Api` value of the API with the specified name.

## Instance methods

### openport (port = 1)

Opens the input or output port with the given 1-based port number.

### getportcount ()

Returns the number of ports available.

### getportname (port = 1)

Returns the name of the given input or output port.

### closeport ()

Closes the currently opened input or output port.

### isportopen ()

Returns true if a port is currently opened, false if not.

### seterrorcallback (f = nil, userdata = nil)

If `f` is not nil, sets it as the callback to be invoked whenever an
error has occurred. `f` should be a callable object accepting the following
arguments: one of the [`RtMidiError.type`](./luartmidi.rtmidierror.md) values,
the error message, and the `userdata` provided.

If `f` is nil, uses RtMidi's default error callback, with no `userdata` object.
It is equivalent to:

```lua
function (typ, errortext, userdata)
  if typ == RtMidiError.Type.WARNING then
    RtMidiError(errortext, typ):printmessage()
  elseif typ ~= RtMidiError.Type.DEBUG_WARNING then
    error(RtMidiError(errortext, typ))
  end
end
```
