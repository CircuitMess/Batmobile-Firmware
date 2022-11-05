#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <BatCommon.h>
#include <Batmobile.h>
#include <CircuitOS.h>
#include <Wheelson.h>
#include <Camera.h>
#include <Loop/LoopManager.h>
#include "src/States/State.h"
#include "src/States/PairState.h"

Sprite *baseSprite;
Display *display;

State* state = nullptr;

void setup() {
    Serial.begin(115200);

    Batmobile.begin();
    Wheelson.begin();

	display = &Wheelson.getDisplay();
    baseSprite = display->getBaseSprite();

    baseSprite->clear(TFT_GREEN);
	display->commit();
    LED.setBacklight(true);

    state = new PairState();
	state->start();
}

void loop() {
    LoopManager::loop();
//    display->commit();
}
