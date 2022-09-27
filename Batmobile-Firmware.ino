#include <Arduino.h>
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