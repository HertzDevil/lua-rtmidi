# luartmidi

The main module for **lua-rtmidi**.

## Constants

### RtMidi

The [`RtMidi`](./luartmidi.rtmidi.md) module, the base class for RtMidiIn and
RtMidiOut.

### RtMidiIn

The [`RtMidiIn`](./luartmidi.rtmidiin.md) class, which is responsible for MIDI
input.

### RtMidiOut

The [`RtMidiOut`](./luartmidi.rtmidiout.md) class, which is responsible for MIDI
output.

### RtMidiError

The [`RtMidiError`](./luartmidi.rtmidierror.md) class, which represents RtMidi
exceptions.

## Module functions

### sleep (sec)

Causes the current thread to sleep for the given number of seconds. This
function is not part of RtMidi and is only provided for convenience.
