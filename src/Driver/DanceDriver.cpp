
#include <Communication/Communication.h>
#include "DanceDriver.h"
#include <Batmobile.h>


DanceDriver::DanceDriver() : Driver(DriveMode::Dance){ }

DanceDriver::~DanceDriver() {
    stop();
}

void DanceDriver::onStart(){
	Com.addListener(ComType::Dance, this);
	LoopManager::addListener(this);
}

void DanceDriver::onStop(){
	Motors.setAll(0);
	Com.removeListener(this);
	LoopManager::removeListener(this);
}

void DanceDriver::onFrame(DriveInfo &driveInfo) {

}

void DanceDriver::onDance(DanceType dance) {

    switch(dance){
        case DanceType::Idle:
            currentDance = DanceType::Idle;
            break;
        case DanceType::Wiggle:
            currentDance = DanceType::Wiggle;
            break;
        case DanceType::BackNFront:
            currentDance = DanceType::BackNFront;
            break;
        case DanceType::Spin:
            currentDance = DanceType::Spin;
            break;
    }
}

void DanceDriver::loop(uint micros) {
    count += micros;
    if(count >= switchDelay){
        count = 0;
        if(danceFlag){
            danceFlag = false;
            Motors.setAll(danceInfo[(uint8_t)currentDance][0]);
        }else{
            danceFlag = true;
            Motors.setAll(danceInfo[(uint8_t)currentDance][1]);
        }
    }
}
