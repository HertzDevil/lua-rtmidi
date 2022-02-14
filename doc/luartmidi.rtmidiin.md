# luartmidi.rtmidiin

MIDI input class.

## Constructor

### RtMidiIn (client_name, api)

Constructs a MIDI input object.
`name` is an optional string to name the application instance and group ports.
`api` is an optional [`RtMidi.Api`](./luartmidi.rtmidi.md) value to choose the API to use.

## Instance methods

Inherits all methods from [`RtMidi`](./luartmidi.rtmidi.md).

### getcurrentapi ()

Returns the [`RtMidi.Api`](./luartmidi.rtmidi.md) value for the current object.

### setcallback (f = nil, userdata = nil)

If `f` is not nil, sets it as the callback to be invoked whenever a MIDI message
has arrived. It should be a callable object accepting the following arguments:
the delta time in seconds since the previous MIDI message, a non-empty sequence
containing the byte values that make up the message, and the `userdata`
provided. **The callback function is invoked from a separate thread, and Lua
does not provide thread safety; do NOT use this unless you are 100% sure what
you are doing to the Lua state!**

If `f` is nil, removes any active callbacks so that incoming MIDI messages are
queued and retrieved using `getmessage`. This is the default behaviour.

### ignoretypes (sysex, time, sense)

Specifies whether the input object ignores system exclusive / timing / active
sensing messages.

### getmessage ()

Retrieves a single MIDI message from the input queue, and returns two values:
the delta time in seconds since the previous MIDI message, followed by a
sequence containing the byte values that make up the message. If no MIDI
messages are available, returns nothing.

This function does not block.
