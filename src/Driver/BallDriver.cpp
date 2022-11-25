#include <Communication/Communication.h>
#include <Loop/LoopManager.h>
#include "BallDriver.h"

BallDriver::BallDriver() {}

BallDriver::~BallDriver() {
    stop();
}

void BallDriver::onStart() {
    Serial.println("BallDriver started");
    Com.addListener({ ComType::DriveSpeed, ComType::BallHue }, this);
    LoopManager::addListener(this);
}

void BallDriver::onStop() {
//    Com.removeListener(this);
    Com.removeListener(ComType::DriveSpeed, this);
    Com.removeListener(ComType::BallHue, this);
    LoopManager::removeListener(this);
}

void BallDriver::loop(uint micros) {
//    Serial.println("I'm in a loop");
}

void BallDriver::onFrame(DriveInfo &driveInfo) {

}

void BallDriver::onBallHue(uint8_t hue) {
    Serial.printf("Ball hue: %d\n", hue);
}

void BallDriver::onDriveSpeed(uint8_t speed) {
    Serial.printf("Drive speed: %d\n", speed);
}
