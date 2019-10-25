# luartmidi.rtmidiout

MIDI output class.

## Constructor

### RtMidiOut ()

Constructs a MIDI output object.

## Instance methods

Inherits all methods from [`RtMidi`](./luartmidi.rtmidi.md).

### getcurrentapi ()

Returns the [`RtMidi.Api`](./luartmidi.rtmidi.md) value for the current object.

### sendmessage (...)

Immediately sends a single MIDI message comprising the given byte values.

This function does not block.

### channel (index)

Returns a [`ChannelOut`](./luartmidi.channelout.md) proxy object that binds to
the MIDI channel with the given channel number `index` between 0 and 15.

### sysex (...)

Composes and sends a system exclusive message. Each argument may be a numeric
value between 0 and 127, or a string containing only byte values between 0 and
127. The arguments are concatenated together and sent as a single message with
the proper system exclusive header and terminator.
