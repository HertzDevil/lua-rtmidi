# luartmidi.rtmidierror

Error class used to report RtMidi exceptions.

## Enumeration types

### Type

```lua
{
	WARNING = 0,
	DEBUG_WARNING = 1,
	UNSPECIFIED = 2,
	NO_DEVICES_FOUND = 3,
	INVALID_DEVICE = 4,
	MEMORY_ERROR = 5,
	INVALID_PARAMETER = 6,
	INVALID_USE = 7,
	DRIVER_ERROR = 8,
	SYSTEM_ERROR = 9,
	THREAD_ERROR = 10,
}
```

The reported error type.

## Constructor

### RtMidiError (message, type = Type.UNSPECIFIED)

Constructs an exception object.

## Instance methods

### printmessage ()

Prints the error message to the standard error stream.

### gettype ()

Returns the error type.

### getmessage ()

Returns the error message.

### what ()

Returns the error message.

## Instance metamethods

### __tostring

Returns the error message.
