# API documentation

* [`luartmidi`](./luartmidi.md)
  * [`ChannelOut`](./luartmidi.channelout.md)
  * [`RtMidi`](./luartmidi.rtmidi.md)
  * [`RtMidiIn`](./luartmidi.rtmidiin.md)
  * [`RtMidiOut`](./luartmidi.rtmidiout.md)
  * [`RtMidiError`](./luartmidi.rtmidierror.md)

## Notes on enumeration types

Both name-value pairs and value-name pairs are available in the enumeration
tables; only the name-value pairs are shown. For example, if an enumeration type
has the following fields:

```lua
{
  x = 0,
  y = 1,
  z = 2,
}
```

Then the following fields are also present:

```lua
{
  [0] = x,
  [1] = y,
  [2] = z,
}
```

All enumerators in this library take unique values, so all value-name pairs
exist.

## Example

The following example code implements a simple MIDI player. It depends on
[MIDI.lua](https://pjb.com.au/comp/lua/MIDI.html).

```lua
local midi = require 'midi'
local luartmidi = require 'luartmidi'

if not arg[1] then
  print('Usage: ' .. arg[-1] .. ' ' .. arg[0] .. ' [midifile]')
  os.exit(1)
end

local midiout = luartmidi.RtMidiOut()
midiout:openport()
assert(midiout:isportopen(), 'Cannot open MIDI output port!')

local channels = {}
for i = 0, 15 do
  channels[i] = midiout:channel(i)
end

local tracks = midi.midi2opus(io.open(arg[1], 'rb'):read('*a'))
local ppqn = table.remove(tracks, 1)
local tempo = 6e7 / 120

for i, tr in ipairs(tracks) do
  tr.pos = 1
end

local t = 0
local talt = 0

while true do
  local wait = math.huge

  for i, tr in ipairs(tracks) do
    while tr.pos <= #tr do
      local event = tr[tr.pos]
      if event[2] > 0 then
        wait = math.min(wait, event[2])
        break
      end
      tr.pos = tr.pos + 1
      if event[1] == 'note_off' then
        channels[event[3]]:noteoff(event[4], event[5])
      elseif event[1] == 'note_on' then
        channels[event[3]]:noteon(event[4], event[5])
      elseif event[1] == 'key_after_touch' then
        channels[event[3]]:aftertouch(event[5], event[4])
      elseif event[1] == 'control_change' then
        channels[event[3]]:controlchange(event[4], event[5])
      elseif event[1] == 'patch_change' then
        channels[event[3]]:programchange(event[4])
      elseif event[1] == 'channel_after_touch' then
        channels[event[3]]:aftertouch(event[4])
      elseif event[1] == 'pitch_wheel_change' then
        channels[event[3]]:pitchbend(event[4] + 0x2000)
      elseif event[1] == 'set_tempo' then
        tempo = event[3]
      end
    end
  end

  local ended = true
  for i, tr in ipairs(tracks) do
    local event = tr[tr.pos]
    if event then
      event[2] = event[2] - wait
      ended = false
    end
  end

  luartmidi.sleep(tempo * 1e-6 / ppqn * wait)
  t = t + wait

  if ended then
    break
  end
end
```
