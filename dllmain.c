#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <SDKDDKVer.h>
#include <windows.h>
#include <stdlib.h>
#include "opl3.h"

#ifdef DLL_EXPORT
#define DLL_EXPORT __declspec(dllimport)
#else
#define DLL_EXPORT __declspec(dllexport)
#endif

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


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

const char* reopl_version_string = "Nuked-OPL3";
static opl3_chip* chip = NULL;
static unsigned int opl_reg_select;
static unsigned int sampleRate;
unsigned int isOPL3model;

/*
	OPL2 registers mapped to $FDE4 and $FDE5 on the SoundX card
	See: https://github.com/SukkoPera/SoundX
	We also allow OPL3 but output is always mono
*/

void DLL_EXPORT oplCreate(unsigned int model, unsigned int replayFreq)
{
	if (!chip)
		chip = (opl3_chip*)malloc(sizeof(opl3_chip));
	isOPL3model = model ? 0x100 : 0;
	sampleRate = replayFreq;
	OPL3_Reset(chip, sampleRate);
}

void DLL_EXPORT oplDestroy()
{
	if (!chip) {
		free(chip);
		chip = NULL;
	}
}

void DLL_EXPORT oplSetSampleRate(unsigned int sampleRate_)
{
	sampleRate = sampleRate_;
}

void DLL_EXPORT oplSetModel(unsigned int model)
{
	isOPL3model = model ? 0x100 : 0;
}

void DLL_EXPORT oplReset()
{
	OPL3_Reset(chip, sampleRate);
}

void DLL_EXPORT oplPause()
{
	//
}

void DLL_EXPORT oplWriteReg(unsigned int reg, unsigned char value)
{
	if (reg >= (isOPL3model | 0xFF))
		return;

	if (reg & 1)
		OPL3_WriteRegBuffered(chip, opl_reg_select, value);
	else
		opl_reg_select = value;
}

unsigned char DLL_EXPORT oplReadReg(unsigned int reg)
{
	if (!(reg & 1) || isOPL3model)
		return OPL3_ReadReg(chip, reg);
	return 0;
}

void DLL_EXPORT oplCalcSamples(short* buf, long count)
{
	if (chip)
		OPL3_GenerateStreamMono(chip, buf, count);
}

short DLL_EXPORT oplCalcSample()
{
	short sample;
	OPL3_GenerateStreamMono(chip, &sample, 1);
	return sample;
}

void DLL_EXPORT oplSetAmp(unsigned int amp)
{

}

const char DLL_EXPORT* oplGetLibVersion()
{
	return reopl_version_string;
}
