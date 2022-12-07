#include "ManualDriver.h"
#include <Communication/Communication.h>
#include <Batmobile.h>
#include <DriveDirection.h>
#include <Loop/LoopManager.h>

ManualDriver::ManualDriver(){
	Com.addListener({ ComType::Boost, ComType::DriveDir }, this);
	LoopManager::addListener(this);
}

ManualDriver::~ManualDriver(){
	Com.removeListener(this);
	LoopManager::removeListener(this);
}

void ManualDriver::onFrame(DriveInfo& driveInfo){

}

void ManualDriver::onBoost(bool boost){
	boosting = boost;
	setMotors();

	if(!boosting){
		drifting = false;
		Audio.play(SPIFFS.open("/SFX/boostOff.aac"));
	}else if(boosting && (parsedDirection == DriveDirection::Forward || parsedDirection == DriveDirection::Backward)){
		drifting = false;
		Audio.play(SPIFFS.open("/SFX/boostOn.aac"));
	}else if(boosting && !(parsedDirection == DriveDirection::Forward || parsedDirection == DriveDirection::Backward)){
		drifting = true;
		Audio.play(SPIFFS.open("/SFX/boostTurn.aac"));
	}
}

void ManualDriver::onDriveDir(uint8_t dir){
	direction = dir;
	directionTimeout = 0;
	setMotors();

	if(drifting && boosting && (parsedDirection == DriveDirection::Forward || parsedDirection == DriveDirection::Backward)){
		drifting = false;
		Audio.play(SPIFFS.open("/SFX/boostOn.aac"));
	}else if(!drifting && boosting && !(parsedDirection == DriveDirection::Forward || parsedDirection == DriveDirection::Backward)){
		drifting = true;
		Audio.play(SPIFFS.open("/SFX/boostTurn.aac"));
	}

}

void ManualDriver::setMotors(){
	float leftSpeed, rightSpeed;

	bool forward = (direction & 0b0001);
	bool backward = (direction & 0b0010);
	bool left = (direction & 0b0100);
	bool right = (direction & 0b1000);

	parsedDirection = DriveDirection::None;
	if(forward == backward){
		if(left != right){
			parsedDirection = left ? DriveDirection::Left : DriveDirection::Right;
		}
	}else if(forward){
		if(left != right){
			parsedDirection = left ? DriveDirection::ForwardLeft : DriveDirection::ForwardRight;
		}else{
			parsedDirection = DriveDirection::Forward;
		}
	}else if(backward){
		if(left != right){
			parsedDirection = left ? DriveDirection::BackwardLeft : DriveDirection::BackwardRight;
		}else{
			parsedDirection = DriveDirection::Backward;
		}
	}


	switch(parsedDirection){
		case DriveDirection::Forward:
			leftSpeed = rightSpeed = speedStraight;
			break;
		case DriveDirection::Backward:
			leftSpeed = rightSpeed = -speedStraight;
			break;
		case DriveDirection::Left:
			leftSpeed = speedTurnInner;
			rightSpeed = speedTurnOuter;
			break;
		case DriveDirection::Right:
			leftSpeed = speedTurnOuter;
			rightSpeed = speedTurnInner;
			break;
		case DriveDirection::ForwardLeft:
			leftSpeed = speedLightTurnInner;
			rightSpeed = speedLightTurnOuter;
			break;
		case DriveDirection::ForwardRight:
			leftSpeed = speedLightTurnOuter;
			rightSpeed = speedLightTurnInner;
			break;
		case DriveDirection::BackwardLeft:
			leftSpeed = -speedLightTurnOuter;
			rightSpeed = -speedLightTurnInner;
			break;
		case DriveDirection::BackwardRight:
			leftSpeed = -speedLightTurnInner;
			rightSpeed = -speedLightTurnOuter;
			break;
		default:
			leftSpeed = rightSpeed = 0;
			break;
	}

	if(!boosting){
		leftSpeed *= noBoostMultiplier;
		rightSpeed *= noBoostMultiplier;
	}

	rightSpeed = std::round(constrain(rightSpeed, -100.0f, 100.0f));
	leftSpeed = std::round(constrain(leftSpeed, -100.0f, 100.0f));

	Motors.setAll({
		(int8_t) rightSpeed, (int8_t) leftSpeed,
		(int8_t) rightSpeed, (int8_t) leftSpeed
	});
}

void ManualDriver::loop(uint micros){
	if(directionTimeout >= directionReceiveInterval && direction != 0){
		direction = 0;
		setMotors();
	}else if(directionTimeout <= directionReceiveInterval){
		directionTimeout += micros;
	}
}
