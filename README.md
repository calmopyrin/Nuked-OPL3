# Nuked-OPL3 DLL Wrapper

This project is a Windows DLL wrapper around **NukeyKT's cycle‑accurate OPL3 (YMF262) emulator**.  
The goal of this fork is to provide a simple and stable C API for applications that need real‑time OPL2/OPL3 FM synthesis, register‑level access, or audio sample generation.

The emulator core is kept mostly unmodified from the original Nuked‑OPL3 project.  
Only "fake" timer emulation (for detection) and a lightweight DLL interface and supporting glue code have been added.

Emulators supporting the DLL:
- YAPE - a Commodore 264 family emulator for Windows (http://yape.homeserver.hu/)
- WinTVC - a Videoton TV Computer emulator for Windows (http://gaia.atilia.eu/downloads.html)

## Features

- Cycle‑accurate Yamaha YMF262 (OPL3) emulation  
- Supports both **OPL2** and **OPL3** modes  
- Real‑time PCM audio generation  
- Register‑accurate read/write interface  
- Adjustable sample rate and amplitude  
- Simple C API suitable for any language that can load a DLL  
- Single‑instance design for easy integration  

## Exported API

```c
void DLL_EXPORT oplCreate(unsigned int model, unsigned int replayFreq);
void DLL_EXPORT oplDestroy();
void DLL_EXPORT oplSetSampleRate(unsigned int sampleRate_);
void DLL_EXPORT oplSetModel(unsigned int model);
void DLL_EXPORT oplReset();
void DLL_EXPORT oplPause();
void DLL_EXPORT oplWriteReg(unsigned int reg, unsigned char value);
unsigned char DLL_EXPORT oplReadReg(unsigned int reg);
void DLL_EXPORT oplCalcSamples(short* buf, long count);
short DLL_EXPORT oplCalcSample();
void DLL_EXPORT oplSetAmp(unsigned int amp);
const DLL_EXPORT char* oplGetLibVersion();
```

## API Description

### Initialization

#### `oplCreate(unsigned int model, unsigned int replayFreq)`
Creates and initializes the OPL emulator instance.

- `model = 0` → OPL2 mode  
- `model = 1` → OPL3 mode  
- `replayFreq` = internal update frequency (typically **49716 Hz**)

#### `oplDestroy()`
Destroys the emulator instance and frees resources.

### Configuration

#### `oplSetSampleRate(unsigned int sampleRate_)`
Sets the output PCM sample rate.

#### `oplSetModel(unsigned int model)`
Switches between OPL2 and OPL3 modes.

#### `oplSetAmp(unsigned int amp)`
Sets output amplitude scaling.

#### `oplReset()`
Resets the chip to its power‑on state.

#### `oplPause()`
Pauses audio generation without destroying state.

### Register Access

#### `oplWriteReg(unsigned int reg, unsigned char value)`
Writes a value to an OPL register.

#### `unsigned char oplReadReg(unsigned int reg)`
Reads back the last written value of an OPL register.

### Audio Generation

#### `oplCalcSamples(short* buf, long count)`
Generates `count` samples of 16‑bit signed PCM audio into `buf`.

#### `short oplCalcSample()`
Generates and returns a single PCM sample.

### Version

#### `const char* oplGetLibVersion()`
Returns a static version string for the DLL.

## Building

The DLL can be built using:

- Microsoft Visual Studio  
- MinGW‑w64  
- Clang‑CL  

No external dependencies are required.

## License

This fork follows the original **Nuked‑OPL3** license (LGPL).
Please refer to the upstream project for licensing details.

## Upstream Project

Nuked‑OPL3 by NukeyKT  
https://github.com/nukeykt/Nuked-OPL3
