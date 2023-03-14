#ifndef BATMOBILE_FIRMWARE_JIGHWTEST_H
#define BATMOBILE_FIRMWARE_JIGHWTEST_H

#include <Display/Display.h>
#include <Util/Vector.h>
#include <Input/Input.h>
#include <FS.h>
#include <Util/Task.h>
#include <JPEGDEC.h>

struct Test {
	bool (* test)();
	const char* name;
	void (* onFail)();
};

class JigHWTest {
public:
	JigHWTest();
	void start();

private:
	static JigHWTest* test;
	Vector<Test> tests;
	const char* currentTest;

	void log(const char* property, const char* value);
	void log(const char* property, float value);
	void log(const char* property, double value);
	void log(const char* property, bool value);
	void log(const char* property, uint32_t value);
	void log(const char* property, int32_t value);
	void log(const char* property, const String& value);

	static bool BatteryCalib();
	static bool BatteryCheck();
	static bool SPIFFSTest();
	static bool S3Test();
	[[noreturn]] void AudioVisualTest();

	static uint32_t calcChecksum(fs::File& file);

	static const int16_t referenceVoltage = 4000;

	//Feed output
	JPEGDEC jpeg;
	uint32_t fetchTime = 0;
	static constexpr uint32_t fetchInterval = 60;

	uint32_t ledMillis = millis();
	uint8_t RGBCounter = 0;
	static constexpr uint32_t LEDInterval = 350;
	static constexpr uint8_t LEDBrightness = 120;

	Pixel* textFrame = nullptr;
};

#endif //BATMOBILE_FIRMWARE_JIGHWTEST_H
