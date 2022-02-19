# Simple J1939

SimpleJ19319 is a communication library that supports sending messages with the J1939 format for the MCP2515 driver. The library is not intended to be a full J1939 stack and instead can be used to communicate with J1939 devices in a simple manner for rapid-prototyping.

The library is based on the work from [Copperhill Technologies](https://copperhilltech.com). In specific the examples located here

https://copperhilltech.com/ard1939-sae-j1939-protocol-stack-for-arduino-teensy-esp32/

What was taken from the examples is the parsing of raw CAN frames for transmission and reception. In addition, the code was refactored into C++ and the MCP driver implementation was updated since the examples were using a deprecated API from the [MCP CAN library](https://github.com/coryjfowler/MCP_CAN_lib).

## Requirements

Installation of the following arduino libraries

- https://github.com/coryjfowler/MCP_CAN_lib


## How to use

Check the examples folder for usage of the API and the examples from [Copperhill Technologies](https://copperhilltech.com) as the implementation is based off their work.


