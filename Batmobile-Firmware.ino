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

    if(Battery.getPercentage() <= 2 && !Battery.charging()){
        Audio.play(SPIFFS.open("/SFX/disconnect.aac"));
        Underlights.breathe( {0, 0 ,0}, {255, 0, 255}, 4000 ,2);
        uint32_t counter = 0;
        while(counter <= 500000){
            Underlights.loop(0);
            counter++;
        }
        Batmobile.shutdown();
        return;
    }

    auto manager = new StateManager();
    manager->begin();
}

void loop() {
    LoopManager::loop();
}
