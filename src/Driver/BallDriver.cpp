#include <Communication/Communication.h>
#include <Loop/LoopManager.h>
#include "BallDriver.h"
#include <Batmobile.h>
#include <ColorConverter.h>


BallDriver::BallDriver(){
    Com.addListener(ComType::BallHue, this);
}

BallDriver::~BallDriver() {
    Com.removeListener(ComType::BallHue, this);
}

void BallDriver::onFrame(DriveInfo &driveInfo) {
    if(driveInfo.mode != DriveMode::Ball) return;
    ballInfo = driveInfo.toBall();

    //NOT TESTED
    /*Ball* bestBall = nullptr;
    uint16_t maxRadius = 0;

    for(auto &ball : ballInfo->balls){
        if(ball.r > maxRadius){
            maxRadius = ball.r;
            bestBall = &ball;
        }
    }

    if(maxRadius >= StopRadius){
        Motors.setAll(0);
        return;
    }

    int currentX = 0;

    if(bestBall == nullptr || maxRadius == 0){
        if(lastX == -1 || noBallCounter > 4){
            Motors.setAll(0);
            return;
        }else{
            noBallCounter++;
            currentX = lastX;
        }
    }else{
        currentX = bestBall->pos.x;
        noBallCounter = 0;
    }

    float amt = abs(80.0 - (float)currentX) / 80.0;
    float amtR, amtL;
    if(amt <= 0.1){
        amtL = 120;
        amtR = 120;
    }else{
        if(currentX < 80){
            amtR = 80.0f * amt + 40.0f;
            amtL = 0;
        }else{
            amtR = 0;
            amtL = 80.0f * amt + 40.0f;
        }
    }
    lastX = currentX;

    Motors.setLeft(amtL);
    Motors.setRight(amtR);*/

}

void BallDriver::onBallHue(uint8_t hue) {
    Underlights.setValue(ColorConverter::toRGB(hue));
    //TODO: send S3 information of the hue
}
