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

	Com.begin();

    if(Battery.getPercentage() < 1 && !Battery.charging()){
        Audio.play(SPIFFS.open("/SFX/disconnect.aac"));
		Underlights.breathe({ 255, 0, 0 }, { 0, 0, 0 }, 6000);
        uint32_t t = millis();
        while(millis() - t <= 3000){
            Underlights.loop(0);
        }
        Batmobile.shutdown();
        return;
    }
	Audio.play(SPIFFS.open("/SFX/booted.aac"));

    auto manager = new StateManager();
    manager->begin();
}

void loop() {
    LoopManager::loop();
}
