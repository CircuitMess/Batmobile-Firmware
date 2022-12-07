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

void MarkerDriver::onFrame(DriveInfo& driveInfo){
	if(current == MarkerAction::Burnout || current == MarkerAction::Do360 || current == MarkerAction::Batsplosion){
		driveInfo.motors = Motors.getAll();
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
	driveInfo.motors = Motors.getAll();
}

void MarkerDriver::processAction(MarkerAction action){
	switch(action){
		case MarkerAction::Forward:
			Motors.setAll({motorsSpeed, motorsSpeed, motorsSpeed, motorsSpeed});
			break;
		case MarkerAction::Backward:
			Motors.setAll({-motorsSpeed, -motorsSpeed, -motorsSpeed, -motorsSpeed});
			break;
		case MarkerAction::HeadlightOn:
			Headlights.setValue(255);
			break;
		case MarkerAction::HeadlightOff:
			Headlights.setValue(0);
			break;
		case MarkerAction::Honk:
			//TODO - playati honk audio
			break;
		case MarkerAction::Batsplosion:
			//TODO - pustiti batsplosion zvuk, jako brzo breathanje Underlightsa crno-narančasto u duljini explosion samplea
			break;
		case MarkerAction::RGBOff:
			Underlights.setValue({ 0, 0, 0 });
			break;
		case MarkerAction::RGBBreathe:
			//TODO - breathanje underlightsa crno-crveno
			break;
		case MarkerAction::RGBSolid:
			Underlights.setValue({ 255, 0, 0 });
			break;
		case MarkerAction::Burnout:
			Motors.setAll({motorsSpeed, motorsSpeed, -motorsSpeed, -motorsSpeed});
			break;
		case MarkerAction::Do360:
			if(rand() % 2){
				Motors.setAll({ motorsSpeed, -motorsSpeed, motorsSpeed, -motorsSpeed });
			}else{
				Motors.setAll({-motorsSpeed, motorsSpeed, -motorsSpeed, motorsSpeed});
			}
			break;
		default:
			Motors.stopAll();
			break;
	}

	current = action;

	if(action == MarkerAction::Burnout || action == MarkerAction::Do360 || action == MarkerAction::Batsplosion){
		LoopManager::addListener(this);
		continuousActionTimer = 0;
	}
}

void MarkerDriver::loop(uint micros){
	continuousActionTimer += micros;

	if((current == MarkerAction::Burnout && continuousActionTimer >= burnoutDuration) ||
	   (current == MarkerAction::Do360 && continuousActionTimer >= do360Duration) ||
	   (current == MarkerAction::Batsplosion && !Audio.isPlaying())){
		continuousActionTimer = 0;
		current = MarkerAction::None;
		Motors.stopAll();
		LoopManager::removeListener(this);
	}else if(!(current == MarkerAction::Burnout || current == MarkerAction::Do360 || current == MarkerAction::Batsplosion)){
		LoopManager::removeListener(this);
	}
}
