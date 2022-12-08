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

	auto manager = new StateManager();
	manager->begin();
}

void loop() {
    LoopManager::loop();
}
