#include <SPIFFS.h>
#include "JigHWTest.h"
#include "SPIFFSChecksum.hpp"
#include <Pins.hpp>
#include <Batmobile.h>
#include <soc/efuse_reg.h>
#include <esp_efuse.h>
#include <Wire.h>
#include <ctime>
#include <Display/Display.h>


Display* display;
Sprite* canvas;

#define TFT_DC MOTOR_BL_A
#define TFT_CS (-1)
#define TFT_SDA MOTOR_BL_B
#define TFT_SCL MOTOR_FL_A
#define TFT_RST MOTOR_FL_B


lgfx::Panel_ST7735S* panel(){
	auto bus = new lgfx::Bus_SPI();
	auto panel = new lgfx::Panel_ST7735S();

	{
		auto cfg = bus->config();


		cfg.spi_host = HSPI_HOST;
		cfg.spi_mode = 0;
		cfg.freq_write = 27000000;
		cfg.freq_read = 27000000;
		cfg.spi_3wire = false;
		cfg.use_lock = true;
		cfg.dma_channel = 1;
		cfg.pin_sclk = TFT_SCL;
		cfg.pin_mosi = TFT_SDA;
		cfg.pin_miso = -1;
		cfg.pin_dc = TFT_DC;

		bus->config(cfg);
		panel->setBus(bus);
	}

	{
		auto cfg = panel->config();

		cfg.pin_cs = TFT_CS;
		cfg.pin_rst = TFT_RST;
		cfg.pin_busy = -1;


		cfg.memory_width = 128;
		cfg.memory_height = 160;
		cfg.panel_width = 128;
		cfg.panel_height = 160;
		cfg.offset_x = 0;
		cfg.offset_y = 0;
		cfg.offset_rotation = 0;
		cfg.readable = true;
		cfg.invert = false;
		cfg.rgb_order = false;
		cfg.dlen_16bit = false;
		cfg.bus_shared = true;


		panel->config(cfg);
	}

	return panel;
}

JigHWTest* JigHWTest::test = nullptr;

JigHWTest::JigHWTest(){

	for(auto pin: { LED_R, LED_G, LED_B }){
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}

	for(auto pin: { MOTOR_FL_A, MOTOR_FR_A, MOTOR_BL_A, MOTOR_BR_A, MOTOR_FL_B, MOTOR_FR_B, MOTOR_BL_B, MOTOR_BR_B }){
		pinMode(pin, OUTPUT);
		digitalWrite(pin, HIGH);
	}


	display = new Display(160, 128);
	canvas = display->getBaseSprite();
	display->getTft()->setPanel(panel());
	display->begin();
	display->getTft()->setRotation(3);
	display->swapBytes(false);

	canvas = display->getBaseSprite();
	canvas->clear(TFT_BLACK);
	display->commit();
	test = this;

	tests.push_back({ JigHWTest::S3Test, "S3 test", [](){}});
	tests.push_back({ JigHWTest::SPIFFSTest, "SPIFFS", [](){}});
	tests.push_back({ JigHWTest::BatteryCalib, "Bat calib", [](){}});
	tests.push_back({ JigHWTest::BatteryCheck, "Bat check", [](){}});
}

void JigHWTest::start(){
	Serial.println();
	Serial.printf("TEST:begin:%llx\n", ESP.getEfuseMac());

	canvas->clear(TFT_BLACK);
	canvas->setTextColor(TFT_GOLD);
	canvas->setTextWrap(false, false);
	canvas->setTextDatum(textdatum_t::middle_center);

	canvas->setTextFont(0);
	canvas->setTextSize(1);
	canvas->setCursor(0, 6);

	canvas->printCenter("BatController test");
	canvas->setCursor(canvas->width() / 2, 16);
	canvas->println();
	display->commit();

	bool pass = true;
	for(const Test& test: tests){
		currentTest = test.name;

		canvas->setTextColor(TFT_WHITE);
		canvas->printf("%s: ", test.name);
		display->commit();

		Serial.printf("TEST:startTest:%s\n", currentTest);

		bool result = test.test();

		canvas->setTextColor(result ? TFT_GREEN : TFT_RED);
		canvas->printf("%s\n", result ? "PASSED" : "FAILED");
		display->commit();

		Serial.printf("TEST:endTest:%s\n", result ? "pass" : "fail");

		if(!(pass &= result)){
			if(test.onFail){
				test.onFail();
			}

			break;
		}
	}

	if(!pass){
		Serial.printf("TEST:fail:%s\n", currentTest);
		for(;;);
	}

	Serial.println("TEST:passall");

//------------------------------------------------------


	canvas->clear(TFT_BLACK);
	canvas->setTextColor(TFT_GOLD);
	canvas->setTextWrap(false, false);
	canvas->setTextDatum(textdatum_t::middle_center);

	canvas->setTextFont(0);
	canvas->setTextSize(1);
	canvas->setCursor(0, 6);

	canvas->printCenter("Batmobile test");
	canvas->setCursor(canvas->width() / 2, 16);
	canvas->println();
	display->commit();
	canvas->print("\n\n");
	canvas->setTextColor(TFT_GREEN);
	canvas->printCenter("All OK!");
	display->commit();

	delay(1000);

	canvas->clear(TFT_BLACK);
	display->commit();

	AudioVisualTest();

}

void JigHWTest::log(const char* property, const char* value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, float value){
	Serial.printf("%s:%s:%f\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, double value){
	Serial.printf("%s:%s:%lf\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, bool value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value ? "TRUE" : "FALSE");
}

void JigHWTest::log(const char* property, uint32_t value){
	Serial.printf("%s:%s:%lu\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, int32_t value){
	Serial.printf("%s:%s:%ld\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, const String& value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value.c_str());
}

bool JigHWTest::BatteryCalib(){
	if(Battery.getVoltOffset() != 0){
		test->log("calibrated", Battery.getVoltOffset());
		display->getBaseSprite()->print("fused. ");
		return true;
	}

	constexpr uint16_t numReadings = 50;
	constexpr uint16_t readDelay = 50;
	uint32_t reading = 0;

	for(int i = 0; i < numReadings; i++){
		reading += analogRead(PIN_BATT);
		delay(readDelay);
	}
	reading /= numReadings;


	uint32_t mapped = Battery.mapReading(reading);

	int16_t offset = referenceVoltage - mapped;

	test->log("reading", reading);
	test->log("mapped", mapped);
	test->log("offset", offset);

	if(abs(offset) >= 1000){
		test->log("offset too big, read voltage: ", (uint32_t) reading);
		return false;
	}

	uint16_t offsetLow = offset & 0b01111111;
	uint16_t offsetHigh = offset >> 7;

	REG_SET_FIELD(EFUSE_BLK3_WDATA3_REG, EFUSE_ADC1_TP_LOW, offsetLow);
	REG_SET_FIELD(EFUSE_BLK3_WDATA3_REG, EFUSE_ADC1_TP_HIGH, offsetHigh);
	esp_efuse_burn_new_values();
	esp_efuse_reset();

	return true;
}

bool JigHWTest::BatteryCheck(){

	constexpr uint16_t numReadings = 50;
	constexpr uint16_t readDelay = 10;
	uint32_t reading = 0;

	for(int i = 0; i < numReadings; i++){
		reading += analogRead(PIN_BATT);
		delay(readDelay);
	}
	reading /= numReadings;

	uint32_t voltage = Battery.mapReading(reading) + Battery.getVoltOffset();
	if(voltage < referenceVoltage - 100 || voltage > referenceVoltage + 100){
		test->log("raw", reading);
		test->log("mapped", Battery.mapReading(reading));
		test->log("offset", Battery.getVoltOffset());
		test->log("mapped+offset", voltage);
		return false;
	}

	return true;
}

bool JigHWTest::SPIFFSTest(){
	if(!SPIFFS.begin()){
		test->log("spiffs", false);
		return false;
	}

	for(const auto& f: SPIFFSChecksums){
		if(!SPIFFS.exists(f.name)){
			test->log("missing", f.name);
			return false;
		}

		fs::File file = SPIFFS.open(f.name, "r");
		uint32_t sum = calcChecksum(file);
		file.close();

		if(sum != f.sum){
			test->log("file", f.name);
			test->log("expected", (uint32_t) f.sum);
			test->log("got", (uint32_t) sum);

			return false;
		}
	}

	return true;
}

uint32_t JigHWTest::calcChecksum(File& file){
	if(!file) return 0;

#define READ_SIZE 512

	uint32_t sum = 0;
	uint8_t b[READ_SIZE];
	size_t read = 0;
	while(read = file.read(b, READ_SIZE)){
		for(int i = 0; i < read; i++){
			sum += b[i];
		}
	}

	return sum;
}

bool JigHWTest::S3Test(){
	if(S3.begin()){
		switch(S3.getError()){
			case S3Error::None:
				return true;
			case S3Error::Camera:
				test->log("Camera check", false);
				return false;
		}
	}else{
		test->log("S3.begin", false);
		return false;
	}
}

void JigHWTest::AudioVisualTest(){
	S3.setMode(DriveMode::Marker);

	disableCore0WDT();
	disableCore1WDT();
//
//	WiFi.mode(WIFI_OFF);
//
	Headlights.begin();
	Taillights.begin();
	Underlights.begin();
//
//	Audio.begin();
//	Audio.setVolume(100);
//	Audio.playRepeating(SPIFFS.open("/Music/music6.aac"));

	for(;;){
		delay(1);

		if(millis() - ledMillis > LEDInterval){
			Headlights.setSolid(RGBCounter % 2 == 0 ? LEDBrightness : 0);
			Taillights.setSolid(RGBCounter % 2 == 0 ? LEDBrightness : 0);
			switch(RGBCounter){
				case 0:
					Underlights.setSolid({ LEDBrightness, 0, 0 });
					break;
				case 1:
					Underlights.setSolid({ 0, LEDBrightness, 0 });
					break;
				default:
					Underlights.setSolid({ 0, 0, LEDBrightness });
					break;
			}
			RGBCounter = (RGBCounter + 1) % 3;
			ledMillis = millis();
		}

		auto t = millis();
		if(t - fetchTime < fetchInterval) continue;
		fetchTime = t;


		auto frame = S3.getFrame();
		if(frame == nullptr){
			printf("Frame nullptr\n");
			continue;
		}

		jpeg.openRAM((uint8_t*) (frame->frame.data), frame->frame.size, [](JPEGDRAW* data) -> int{
			for(int y = data->y, iy = 0; y < data->y + data->iHeight; y++, iy++){
				size_t offset = y * 160 + data->x;
				size_t ioffset = iy * data->iWidth;
				memcpy((uint8_t*) data->pUser + offset * 2, (uint8_t*) data->pPixels + ioffset * 2, data->iWidth * 2);
			}
			return 1;
		});

		jpeg.setUserPointer(canvas->frameBuffer(0));
		jpeg.setPixelType(RGB565_BIG_ENDIAN);

		if(jpeg.decode(0, 0, 0) == 0){
			printf("decode error: %d", jpeg.getLastError());
		}

		auto marker = frame->toMarker();
		if(marker && !marker->markers.empty()){
			for(const auto& m: marker->markers){
				printf("Got marker %d\n", m.id);
				const auto& projected = m.projected;
				for(int i = 0; i < 5; i++){
					canvas->drawLine(projected[i % 4].x, projected[i % 4].y, projected[(i + 1) % 4].x, projected[(i + 1) % 4].y, TFT_RED);
				}

				canvas->setCursor((projected[0].x + projected[1].x) / 2 - 5, projected[0].y - 10);
				canvas->printf("%d", m.id);
			}
		}

		display->commit();
	}
}


