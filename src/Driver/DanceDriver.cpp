
#include <Communication/Communication.h>
#include "DanceDriver.h"
#include <Batmobile.h>

static const MotorInfo danceInfo[5][2] = {{{ 0,    0,   0,    0 },    { 0,   0,    0,   0 }},
								   {{ 40,   -40, 40,   -40 },  { -40, 40,   -40, 40 }},
								   {{ 60,   60,  60,   60 },   { -60, -60,  -60, -60 }},
								   {{ -100, 100, -100, 100 },  { 100, -100, 100, -100 }},
								   {{ -65,  -45, -65,  -45 }, { -45, -65,  -45, -65 }}};
static const glm::vec3 colors[5][2] = {{{ 0,   0,   0 },   { 0,   0,   0 }},
								{{ 65,  105, 225 }, { 255, 229, 180 }}, //royal blue & peach
								{{ 54,  69,  79 },  { 255, 255, 0 }},  //Charcoal & yellow
								{{ 120, 190, 33 },  { 89,  203, 232 }}, //Lime green & electric blue
								{{ 255, 240, 245 }, { 0,   128, 128 }}, }; //Lavender & teal

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
    currentDance = dance;

	count = 0;
	danceFlag = false;
	Motors.setAll(danceInfo[(uint8_t)currentDance][0]);
	Underlights.blinkContinuous(colors[(uint8_t)currentDance][0]);
}

void DanceDriver::loop(uint micros) {
    count += micros;
    if(count >= switchDelay){
        count = 0;
        if(danceFlag){
            danceFlag = false;
            Motors.setAll(danceInfo[(uint8_t)currentDance][0]);
			Underlights.blinkContinuous(colors[(uint8_t)currentDance][0]);
		}else{
            danceFlag = true;
            Motors.setAll(danceInfo[(uint8_t)currentDance][1]);
			Underlights.blinkContinuous(colors[(uint8_t)currentDance][1]);
        }
    }
}
