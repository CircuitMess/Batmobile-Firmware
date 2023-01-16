#include "SimpleProgDriver.h"
#include <Batmobile.h>
#include <SimpleProgColors.h>

SimpleProgDriver::SimpleProgDriver(){
	Com.addListener({ ComType::DriveDir, ComType::DriveSpeed, ComType::Headlights, ComType::Taillights, ComType::Underlights, ComType::SoundEffect }, this);
}

SimpleProgDriver::~SimpleProgDriver(){
	Com.removeListener(this);
}

void SimpleProgDriver::onFrame(DriveInfo& driveInfo){

}

void SimpleProgDriver::onDriveDir(uint8_t dir){
	direction = dir;
	setMotors();
}

void SimpleProgDriver::onDriveSpeed(uint8_t speed){
	speed = constrain(speed, 0, 100);
	this->speed = speed / 100.0;
}

void SimpleProgDriver::onHeadlights(uint8_t val){
	Headlights.setValue(val);
}

void SimpleProgDriver::onTaillights(uint8_t val){
	Taillights.setValue(val);
}

void SimpleProgDriver::onUnderlights(uint8_t color){
	color = constrain(color, 0, NumColors);
	Underlights.setValue(SimpleProgColors[color]);
}

void SimpleProgDriver::onSoundEffect(uint8_t sample){
	File root = SPIFFS.open("/SFX/");
	File f;
	for(int j = 0; j < sample + 1; ++j){
		f = root.openNextFile();
		if(!f) return;
	}
	Audio.play(f);
}

void SimpleProgDriver::setMotors(){
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

	leftSpeed *= speed;
	rightSpeed *= speed;

	rightSpeed = std::round(constrain(rightSpeed, -100.0f, 100.0f));
	leftSpeed = std::round(constrain(leftSpeed, -100.0f, 100.0f));

	Motors.setAll({
						  (int8_t) rightSpeed, (int8_t) leftSpeed,
						  (int8_t) rightSpeed, (int8_t) leftSpeed
				  });
}

