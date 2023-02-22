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

//Platform compiler should be C++17 compatible, but errors show up when removing this
constexpr std::array<const char*, 13> MarkerDriver::audioFiles;

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
		if(!Audio.isPlaying() || (Audio.isPlaying() && lastPlayed != MarkerAction::None)){
			Audio.play(SPIFFS.open("/Voice/unknown.aac"));
			lastPlayed = MarkerAction::None;
		}
		return;
	}

	processAction(ActionMap.at(id));

	driveInfo.toMarker()->action = current;
}

void MarkerDriver::processAction(MarkerAction action){
	switch(action){
		case MarkerAction::Forward:
			Motors.setAll({ motorsSpeed, motorsSpeed, motorsSpeed, motorsSpeed });
			break;
		case MarkerAction::Backward:
			Motors.setAll({ -motorsSpeed, -motorsSpeed, -motorsSpeed, -motorsSpeed });
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
			break;
		case MarkerAction::Batsplosion:
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
			delayedVoiceDone = false;
			break;
		case MarkerAction::Do360:
			delayedVoiceDone = false;
			break;
		case MarkerAction::Bats:
			break;
		default:
			Motors.stopAll();
			break;
	}

	auto audioFile = audioFiles.at((uint8_t) action);
	if(audioFile != nullptr && (!Audio.isPlaying() || (Audio.isPlaying() && lastPlayed != action))){
		Audio.play(SPIFFS.open(audioFile));
		lastPlayed = action;
	}

	current = action;

	if(continuousAction(action)){
		LoopManager::addListener(this);
		continuousActionTimer = 0;
	}
}

void MarkerDriver::loop(uint micros){
	continuousActionTimer += micros;

	//Starting motors is delayed since voicelines aren't audible when motors are running.
	if(current == MarkerAction::Burnout && !Audio.isPlaying() && !delayedVoiceDone){
		delayedVoiceDone = true;
		Motors.setAll({ motorsSpeed, motorsSpeed, -motorsSpeed, -motorsSpeed });
	}else if(current == MarkerAction::Do360 && !Audio.isPlaying() && !delayedVoiceDone){
		delayedVoiceDone = true;
		if(rand() % 2){
			Motors.setAll({ motorsSpeed, -motorsSpeed, motorsSpeed, -motorsSpeed });
		}else{
			Motors.setAll({ -motorsSpeed, motorsSpeed, -motorsSpeed, motorsSpeed });
		}
	}


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
