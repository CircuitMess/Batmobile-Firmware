#include "MarkerDriver.h"
#include <map>
#include <Loop/LoopManager.h>
#include <Batmobile.h>

static const std::map<uint16_t, MarkerAction> ActionMap = {
		{ 750, MarkerAction::Forward },
		{ 686, MarkerAction::Backward },
		{ 5,   MarkerAction::Do360 }, //continuous
		{ 785, MarkerAction::Burnout }, //continuous
		{ 636, MarkerAction::HeadlightOn },
		{ 576, MarkerAction::HeadlightOff },
		{ 409, MarkerAction::Honk },
		{ 341, MarkerAction::Batsplosion }, //continuous
		{ 305, MarkerAction::RGBOff },
		{ 204, MarkerAction::RGBBreathe },
		{ 152, MarkerAction::RGBSolid },
		{ 100, MarkerAction::Bats }
};

const glm::vec<3, uint16_t> MarkerDriver::Colors[8] = {
		{ 0,   0,   0 },
		{ 255, 0,   0 },
		{ 0,   255, 0 },
		{ 0,   0,   255 },
		{ 255, 255, 0 },
		{ 255, 0,   255 },
		{ 0,   255, 255 },
		{ 255, 255, 255 },
};

MarkerDriver::MarkerDriver() : Driver(DriveMode::Marker){}

MarkerDriver::~MarkerDriver(){
	stop();
}

void MarkerDriver::onFrame(DriveInfo& driveInfo){
	if(continuousAction(current)){
		driveInfo.toMarker()->action = current;
		return;
	}

	auto markersInfo = driveInfo.toMarker();

	if(!markersInfo){
		processAction(MarkerAction::None);
		return;
	}

	if(markersInfo->markers.empty()){
		processAction(MarkerAction::None);
		return;
	}

	auto id = markersInfo->markers[0].id;
	if(!ActionMap.count(id)){
		processAction(MarkerAction::None);
		return;
	}

	processAction(ActionMap.at(id));

	driveInfo.toMarker()->action = current;
}

void MarkerDriver::processAction(MarkerAction action){
	switch(action){
		case MarkerAction::Forward:
			if(!motorsLocked){
				Motors.setAll({ motorsSpeed, motorsSpeed, motorsSpeed, motorsSpeed });
			}
			break;
		case MarkerAction::Backward:
			if(!motorsLocked){
				Motors.setAll({ -motorsSpeed, -motorsSpeed, -motorsSpeed, -motorsSpeed });
			}
			break;
		case MarkerAction::HeadlightOn:
			Headlights.setSolid(255);
			Taillights.setSolid(255);
			break;
		case MarkerAction::HeadlightOff:
			Headlights.setSolid(0);
			Taillights.setSolid(0);
			break;
		case MarkerAction::Honk:
			if(!Audio.isPlaying()){
				Audio.play(SPIFFS.open("/SFX/honk.aac"));
			}
			break;
		case MarkerAction::Batsplosion:
			if(!Audio.isPlaying()){
				Audio.play(SPIFFS.open("/SFX/explosion.aac"));
			}
			//TODO - jako brzo breathanje Underlightsa crno-narančasto u duljini explosion samplea
			break;
		case MarkerAction::RGBOff:
			Underlights.setSolid({ 0, 0, 0 });
			breathing = false;
			break;
		case MarkerAction::RGBBreathe:
			if(breathing) break;
			Underlights.breathe({ 0, 255, 255 }, { 255, 255, 100 }, 2000);
			breathing = true;
			break;
		case MarkerAction::RGBSolid:
			if(millis() - colorTime < ColorDuration) break;
			colorTime = millis();

			color = (color + 1) % (sizeof(Colors) / sizeof(Colors[0]));
			Underlights.setSolid(Colors[color]);
			breathing = false;

			break;
		case MarkerAction::Burnout:
			if(!motorsLocked){
				Motors.setAll({ motorsSpeed, motorsSpeed, -motorsSpeed, -motorsSpeed });
			}
			break;
		case MarkerAction::Do360:
			if(!motorsLocked){
				if(rand() % 2){
					Motors.setAll({ motorsSpeed, -motorsSpeed, motorsSpeed, -motorsSpeed });
				}else{
					Motors.setAll({ -motorsSpeed, motorsSpeed, -motorsSpeed, motorsSpeed });
				}
			}
			break;
		case MarkerAction::Bats:
			if(!Audio.isPlaying()){
				Audio.play(SPIFFS.open("/SFX/bats.aac"));
			}
			break;
		default:
			Motors.stopAll();
			break;
	}

	current = action;

	if(continuousAction(action)){
		LoopManager::addListener(this);
		continuousActionTimer = 0;
	}
}

void MarkerDriver::loop(uint micros){
	continuousActionTimer += micros;

	if((current == MarkerAction::Burnout && continuousActionTimer >= burnoutDuration) ||
	   (current == MarkerAction::Do360 && continuousActionTimer >= do360Duration) ||
	   (current == MarkerAction::Batsplosion && continuousActionTimer >= explosionDuration) ||
	   (current == MarkerAction::Bats && continuousActionTimer >= batsDuration)){
		continuousActionTimer = 0;
		current = MarkerAction::None;
		Motors.stopAll();
		LoopManager::removeListener(this);
	}else if(!continuousAction(current)){
		LoopManager::removeListener(this);
	}
}

bool MarkerDriver::continuousAction(MarkerAction action){
	return action == MarkerAction::Burnout || action == MarkerAction::Do360 || action == MarkerAction::Batsplosion || action == MarkerAction::Bats;
}

void MarkerDriver::onMotorsTimeoutClear(){
	motorsLocked = false;
}

void MarkerDriver::onMotorsTimeout(uint8_t duration){
	Motors.stopAll();
	motorsLocked = true;
}

void MarkerDriver::onStart(){
	Com.addListener({ComType::MotorsTimeout, ComType::MotorsTimeoutClear}, this);
}

void MarkerDriver::onStop(){
	Com.removeListener(this);
}
