
#include <Communication/Communication.h>
#include "DanceDriver.h"
#include <Batmobile.h>


static constexpr MotorInfo danceInfo[6][2] = {{{ 0,    0,   0,    0 },   { 0,    0,    0,   0 }}, //Idle
											  {{ 60,   -60, 80,   -80 }, { -60,  60,   -80, 80 }}, //Wiggle
											  {{ 60,   60,  60,   60 },  { -60,  -60,  -60, -60 }}, //BackNFront
											  {{ -100, 100, -100, 100 }, { 100,  -100, 100, -100 }}, //Spin
											  {{ -50,  -50, -30,  -20 }, { -50,  -50,  -20, -30 }}, //BackItUp
											  {{ 25,   100,  35,  100 }, { -100, -25, -100, -35 }}};  //Waltz

static constexpr glm::vec3 colors[6][2] = {{{ 0,   0,   0 },   { 0,   0,   0 }},
										   {{ 65,  105, 225 }, { 255, 229, 180 }}, //royal blue & peach
										   {{ 54,  69,  79 },  { 255, 255, 0 }},  //Charcoal & yellow
										   {{ 120, 190, 33 },  { 89,  203, 232 }}, //Lime green & electric blue
										   {{ 255, 240, 245 }, { 0,   128, 128 }}, //Lavender & teal
										   {{ 137, 207, 240 }, { 255, 255, 255 }}, }; //Baby blue & white

DanceDriver::DanceDriver() : Driver(DriveMode::Dance){}

DanceDriver::~DanceDriver(){
	stop();
}

void DanceDriver::onStart(){
	controls.setLightshowDisable(false);
	Com.addListener(ComType::Dance, this);
	LoopManager::addListener(this);
}

void DanceDriver::onStop(){
	Motors.setAll(0);
	Com.removeListener(this);
	LoopManager::removeListener(this);
}

void DanceDriver::onFrame(DriveInfo& driveInfo){

}

void DanceDriver::onDance(DanceType dance){
	currentDance = dance;

	count = 0;
	danceFlag = false;
	Motors.setAll(danceInfo[(uint8_t) currentDance][0]);
	Underlights.blinkContinuous(colors[(uint8_t) currentDance][0]);
	Audio.play(SPIFFS.open("/SFX/beep.aac"));
}

void DanceDriver::loop(uint micros){
	count += micros;
	if(count >= switchDelay){
		count = 0;
		if(danceFlag){
			danceFlag = false;
			Motors.setAll(danceInfo[(uint8_t) currentDance][0]);
			Audio.play(SPIFFS.open("/SFX/beep.aac"));
			Underlights.blinkContinuous(colors[(uint8_t) currentDance][0]);
		}else{
			danceFlag = true;
			Motors.setAll(danceInfo[(uint8_t) currentDance][1]);
			Audio.play(SPIFFS.open("/SFX/beep.aac"));
			Underlights.blinkContinuous(colors[(uint8_t) currentDance][1]);
		}
	}
}
