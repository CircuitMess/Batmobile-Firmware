
#include <Communication/Communication.h>
#include "DanceDriver.h"
#include <Batmobile.h>


static constexpr MotorInfo danceInfo[9][2] = {{{ 0,    0,   0,    0 },   { 0,    0,    0,    0 }}, //Idle
											  {{ 60,   -60, 80,   -80 }, { -60,  60,   -80,  80 }}, //Wiggle
											  {{ 60,   60,  60,   60 },  { -60,  -60,  -60,  -60 }}, //BackNFront
											  {{ -100, 100, -100, 100 }, { 100,  -100, 100,  -100 }}, //Spin
											  {{ -50,  -50, -30,  -20 }, { -50,  -50,  -20,  -30 }}, //BackItUp
											  {{ 25,   100, 35,   100 }, { -100, -25,  -100, -35 }}, //Waltz
											  {{ 90,   40,  90,   40 },  { 90,   40,  90,   40 }}, //WaxOn
											  {{ 40,   90,  40,   90 },  { 40,   90,   40,   90 }}, //WaxOff
											  {{ 35,   35,  35,   35 },  { 0,    0,    0,    0 }}};     //Sneak

static constexpr glm::vec3 colors[9][2] = {{{ 0,   0,   0 },   { 0,   0,   0 }},
										   {{ 65,  105, 225 }, { 255, 229, 180 }}, //royal blue & peach
										   {{ 54,  69,  79 },  { 255, 255, 0 }},  //Charcoal & yellow
										   {{ 120, 190, 33 },  { 89,  203, 232 }}, //Lime green & electric blue
										   {{ 255, 240, 245 }, { 0,   128, 128 }}, //Lavender & teal
										   {{ 137, 207, 240 }, { 255, 255, 255 }}, //Baby blue & white
										   {{ 218, 112, 214 }, { 222, 192, 95 }}, //Orchid blue & Cream Gold
										   {{ 120, 81,  169 }, { 199, 211, 212 }}, //Royal Purple & Ice Flow
										   {{ 214, 157, 48 },  { 97,  98,  71 }}}; //Mango Mojito & Terrarium Moss

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
	if(lightshow) lightshow->stop();
	lightshow = Lightshow::createLightshow((LightshowType) (uint8_t) currentDance);
	if(lightshow) lightshow->start();
	Audio.playRepeating(SPIFFS.open(String("/Music/music")+ (uint8_t) currentDance + ".aac"));
}

void DanceDriver::loop(uint micros){
	count += micros;
	if(count >= switchDelay){
		count = 0;
		if(danceFlag){
			danceFlag = false;
			Motors.setAll(danceInfo[(uint8_t) currentDance][0]);
		}else{
			danceFlag = true;
			Motors.setAll(danceInfo[(uint8_t) currentDance][1]);
		}
	}
}
