#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <BatCommon.h>
#include <Batmobile.h>
#include <CircuitOS.h>
#include <Loop/LoopManager.h>
#include "src/States/StateManager.h"

void setup() {
    Serial.begin(115200);

	esp_log_level_set("*", ESP_LOG_NONE);

    Batmobile.begin();

    if(Battery.getPercentage() < 1 && !Battery.charging()){
        Batmobile.shutdownNotify();
        return;
    }

	if(S3.hasError()){
		Audio.play(SPIFFS.open("/SFX/disconnect.aac"));

		uint32_t t = millis();
		while(millis() - t < 12000){
			Underlights.setSolid({ 255, 0, 0 });
			delay(500);
			Underlights.setSolid({ 0, 0, 255 });
			delay(500);
		}

		Batmobile.shutdown();
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
