# luartmidi.channel

Proxy object that contains several useful methods to compose MIDI messages that
are sent over a single MIDI channel.

## Constructor

See [`RtMidiOut#channel`](./luartmidi.rtmidiout.md).

## Instance methods

### number ()

Returns the channel number of the object, between 0 and 15.

### noteoff (pitch, velocity = 0)

Sends a note off message with the given MIDI pitch and velocity, both between 0
and 127.

### noteon (pitch, velocity)

Sends a note on message with the given MIDI pitch and velocity, both between 0
and 127. If velocity is equal to 0, the resulting message is equivalent to a
note off.

### aftertouch (pressure [, pitch])

Sends an aftertouch message with the given pressure value between 0 and 127. A
key aftertouch message is sent if the MIDI pitch is given, a channel aftertouch
message is sent otherwise.

### controlchange (cc, value)

Changes the value of a single continuous controller. `cc` must be between 0 and
119; `value` must be between 0 and 127. If `value` is a boolean value, `true`
and `false` are interpreted as 127 and 0 respectively.

### controlchange (cc, msb, lsb)

Changes the MSB and LSB values of a continuous controller, sending the MSB
first. `msb` and `lsb` must both be between 0 and 127, unless `lsb` is equal to
the boolean `true`, in which case `msb` has a range of 0 to 16383, and the LSB
is calculated automatically. `cc` must be between 0 and 31.

### programchange (program)

Sends a program change message with the given program number between 0 and 127.

### pitchbend (position)

Sends a pitch bend message with the given pitch bend position between 0 and
16383 (8192 indicates no change).

### allsoundoff ()

Sends the All Sound Off message (`78 00`).

### resetallcontrollers ()

Sends the Reset All Controllers message (`79 00`).

### localcontrol (enable)

Sends the Local Control message (`7A 7F 00` / `7A 00 00`) depending on whether
`enable` evaluates to true.

### allnotesoff ()

Sends the All Notes Off message (`7B 00`).

### omni (enable)

Turns Omni mode on (`7D 00`) or off (`7C 00`).

### mono (channels)

Enables the Mono mode (`7E xx`), where `channels` is the number of mono channels
from 1 to 16, or 0 which means using all remaining channels.

### poly ()

Enables the Poly mode (`7F 00`).

### rpn (number, value)

Sends a Registered Parameter message with the given parameter number and value,
both between 0 and 16383.

### nrpn (number, value)

Sends a Non-Registered Parameter message with the given parameter number and
value, both between 0 and 16383.
