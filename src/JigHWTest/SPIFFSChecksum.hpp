#ifndef BATMOBILE_FIRMWARE_SPIFFSCHECKSUM_H
#define BATMOBILE_FIRMWARE_SPIFFSCHECKSUM_H

#include <Arduino.h>

struct {
	String name;
	uint32_t sum;
} static const SPIFFSChecksums[] PROGMEM = {
		{ "/Music/music1.aac",            11175356 },
		{ "/Music/music2.aac",            17383391 },
		{ "/Music/music3.aac",            95625132 },
		{ "/Music/music4.aac",            4373481 },
		{ "/Music/music5.aac",            19997293 },
		{ "/Music/music6.aac",            8222118 },
		{ "/Music/music7.aac",            6066454 },
		{ "/Music/music8.aac",            3500035 },
		{ "/SFX/bats.aac",                1693078 },
		{ "/SFX/beep.aac",                572964 },
		{ "/SFX/boostOff.aac",            1823906 },
		{ "/SFX/boostOn.aac",             1996256 },
		{ "/SFX/boostTurn.aac",           1271417 },
		{ "/SFX/booted.aac",              2062891 },
		{ "/SFX/disconnect.aac",          507670 },
		{ "/SFX/driverExit.aac",          1267159 },
		{ "/SFX/driverStart.aac",         731380 },
		{ "/SFX/error.aac",               776913 },
		{ "/SFX/explosion.aac",           6342265 },
		{ "/SFX/honk0.aac",               380874 },
		{ "/SFX/honk1.aac",               550994 },
		{ "/SFX/honk2.aac",               471230 },
		{ "/SFX/honk3.aac",               705662 },
		{ "/SFX/honk4.aac",               760913 },
		{ "/SFX/honk5.aac",               3298179 },
		{ "/SFX/honk6.aac",               737071 },
		{ "/SFX/idle1.aac",               1108449 },
		{ "/SFX/idle2.aac",               1599854 },
		{ "/SFX/idle3.aac",               676554 },
		{ "/SFX/idle4.aac",               2275978 },
		{ "/SFX/off.aac",                 4804128 },
		{ "/SFX/paired.aac",              1121486 },
		{ "/SFX/scan.aac",                696155 },
		{ "/Voice/360.aac",               1708342 },
		{ "/Voice/autopilot.aac",         1367047 },
		{ "/Voice/burnout.aac",           1670227 },
		{ "/Voice/headlightOff.aac",      981624 },
		{ "/Voice/headlightOn.aac",       958671 },
		{ "/Voice/manual.aac",            1466400 },
		{ "/Voice/underlightBreathe.aac", 1171173 },
		{ "/Voice/underlightOff.aac",     1077431 },
		{ "/Voice/underlightSolid.aac",   1060705 },
		{ "/Voice/unknown.aac",           1779002 }
};

#endif //BATMOBILE_FIRMWARE_SPIFFSCHECKSUM_H
