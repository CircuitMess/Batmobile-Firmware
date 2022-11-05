#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <BatCommon.h>
#include <Batmobile.h>
#include <CircuitOS.h>
#include <Wheelson.h>
#include <Camera.h>
#include <Loop/LoopManager.h>
#include "src/States/StateManager.h"

Sprite *baseSprite;
Display *display;

void setup() {
    Serial.begin(115200);

    Batmobile.begin();
    Wheelson.begin();

	display = &Wheelson.getDisplay();
    baseSprite = display->getBaseSprite();

    baseSprite->clear(TFT_GREEN);
	display->commit();
    LED.setBacklight(true);

	auto manager = new StateManager();
	manager->begin();
}

void loop() {
    LoopManager::loop();
//    display->commit();
}
