#include "ManualDriver.h"
#include <Communication/Communication.h>
#include <Batmobile.h>

ManualDriver::ManualDriver(){
	Com.addListener({ComType::Boost, ComType::DriveDir}, this);
}

void ManualDriver::onFrame(DriveInfo& driveInfo){

}

void ManualDriver::onBoost(bool boost){
	boosting = boost;
	setMotors();
}

void ManualDriver::onDriveDir(DriveDirection dir){
	direction = dir;
	setMotors();
}

void ManualDriver::setMotors(){
	int8_t leftSpeed, rightSpeed;

	switch(direction){
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
		default:
			leftSpeed = rightSpeed = 0;
			break;
	}

	if(!boosting){
		leftSpeed *= noBoostMultiplier;
		rightSpeed *= noBoostMultiplier;
	}

	Motors.setAll({rightSpeed, leftSpeed, rightSpeed, leftSpeed});
}
