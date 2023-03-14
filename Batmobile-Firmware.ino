#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <BatCommon.h>
#include <Batmobile.h>
#include <CircuitOS.h>
#include <Loop/LoopManager.h>
#include "src/States/StateManager.h"
#include "src/JigHWTest/JigHWTest.h"

bool checkJig(){
	char buf[7];
	int wp = 0;

	uint32_t start = millis();
	while(millis() - start < 500){
		while(Serial.available()){
			buf[wp] = Serial.read();
			wp = (wp + 1) % 7;

			for(int i = 0; i < 7; i++){
				int match = 0;
				static const char* target = "JIGTEST";

				for(int j = 0; j < 7; j++){
					match += buf[(i + j) % 7] == target[j];
				}

				if(match == 7) return true;
			}
		}
	}

	return false;
}

void setup() {
    Serial.begin(115200);

	esp_log_level_set("*", ESP_LOG_NONE);

	if(checkJig()){
		printf("Jig\n");
		auto test = new JigHWTest();
		test->start();
		for(;;);
	}

    Batmobile.begin();

    if(Battery.getPercentage() < 1 && !Battery.charging()){
        Batmobile.shutdownNotify();
        return;
    }

	if(S3.getError() != S3Error::None){
		Batmobile.shutdownError();
		return;
	}

	Com.begin();
	Audio.play(SPIFFS.open("/SFX/booted.aac"));

    auto manager = new StateManager();
    manager->begin();
}

void loop() {
    LoopManager::loop();
}
