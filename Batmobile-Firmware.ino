#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <BatCommon.h>
#include <Batmobile.h>
#include <CircuitOS.h>
#include <Wheelson.h>
#include <Camera.h>
#include <Loop/LoopManager.h>
#include "src/States/PairService.h"

Sprite *baseSprite;
Display *display;
Pair::PairService* pairService;
void setup() {
    Serial.begin(115200);
    Nuvo.begin();

    Batmobile.begin();
    Wheelson.begin();

    display = &Wheelson.getDisplay();
    baseSprite = display->getBaseSprite();
    baseSprite->clear(TFT_GREEN);
    LED.setBacklight(true);
    pairService = new Pair::PairService();
}

void loop() {
    LoopManager::loop();
    display->commit();
}