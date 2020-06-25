# EncoderEmulator
Encoder Emulator using Arduino by I2C.

The Arduino Nano is used to emulate the quadrature signal (A and B) of an incremental encoder.  

* Fases sequences of the encoder cycle (AB):  
  * Forward: 00 -> 01 -> 11 -> 10 
  * backward: 00 -> 10 -> 11 -> 01 


Signals pins in arduino:  
```
A = pin2
B = pin3
```

## I2C
Commands and parameters (Ctrl Word) are received by I2C.  
The _Ctrl Word_ is received in array of byte with 10 elements where:
```
[0] = Cmd (Byte)
[1..2] = Rpm (Int)
[3..4] = Resolution (Int)
[5..9] = not defined
```

* __I2C address = 8__

| Item | Type | Values | Description |
| ---- | ---- | ------ | ----------- |
| Cmd | Byte | 0= Off<br>1= Forward<br>2= Backward | Command |
| Rpm | Int (16bits) | 0 ~ 16384 |speed _(rot/min)_ to be emulated by the encoder, 1 rotation is equal a cycle of the encoder (4 fases) |
| Resolution | Int (16bits) | 0 ~ 16384 | resolution of the encoder (pulses/rot) |

__IMPORTANT:__ The emulation signal is generated changing the fase of encoder by timer interrupts, this time is set considering RPM and Resolution where 
__time = 60000000 / RPM / Resolution__. The minimum time value is 20, so, any RPM and Resolution configuration resulting less then this value will be set automaticly to 20.

## Codesys
To command the encoder emulator, there is example of Codesys V3 project in the _./Example_ folder, in this project is used a Raspberry Pi with Codesys runtime. The Arduino is connected with the Raspberry Pi by I2C.  
The encoder is inserted in the Device Tree of the project as an I2C device. The _library_ and _XML device description_ file are in ./Codesys folder.  

* Install on Codesys the XML device in Tools > Device Repository ...
* Intall de library in Tools > Library Repository ...

Can be necessary to add manually the library in Library Manager of the Device Tree