#include "SimpleProgDriver.h"
#include <Batmobile.h>
#include <Loop/LoopManager.h>

SimpleProgDriver::SimpleProgDriver() : Driver(DriveMode::SimpleProgramming){
	Com.addListener({ ComType::DriveDir, ComType::DriveSpeed, ComType::Headlights, ComType::Taillights, ComType::Underlights, ComType::SoundEffect,
					  ComType::MotorsTimeout, ComType::MotorsTimeoutClear }, this);
}

SimpleProgDriver::~SimpleProgDriver(){
	Com.removeListener(this);
}

void SimpleProgDriver::onFrame(DriveInfo& driveInfo){

}

void SimpleProgDriver::onDriveDir(uint8_t dir, uint8_t speed){
	direction = (DriveDirection) dir;
	setMotors();
}

void SimpleProgDriver::onDriveSpeed(uint8_t speed){
	speed = constrain(speed, 0, 100);
	this->speed = speed / 100.0;
}

void SimpleProgDriver::onHeadlights(uint8_t val){
	Headlights.setSolid(val);
}

void SimpleProgDriver::onTaillights(uint8_t val){
	Taillights.setSolid(val);
}

void SimpleProgDriver::onUnderlights(uint8_t color){
	glm::vec<3, uint8_t> c = { 255, 255, 255 };
	c *= glm::vec3{ color & 1, color & 2, color & 4 };
	Underlights.setSolid(c);
}

void SimpleProgDriver::onSoundEffect(uint8_t sample){
	File root = SPIFFS.open("/SFX");
	File f = root.openNextFile();
	for(int j = 0; j < sample + 1; ++j){
		f = root.openNextFile();
		if(!f) return;
	}
	Audio.play(f);
}

void SimpleProgDriver::setMotors(){
	float leftSpeed, rightSpeed;

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

void SimpleProgDriver::loop(uint micros){
	if(motorsTimeout == 0) return;

	timer += micros;
	if(timer >= motorsTimeout){
		timer = 0;
		direction = DriveDirection::None;
		setMotors();
	}
}

void SimpleProgDriver::onStart(){
	LoopManager::addListener(this);
}

void SimpleProgDriver::onStop(){
	LoopManager::removeListener(this);
}

void SimpleProgDriver::onMotorsTimeout(uint8_t duration){
	motorsTimeout = duration * 100 * 1000;
	timer = 0;
}

void SimpleProgDriver::onMotorsTimeoutClear(){
	motorsTimeout = 0;
	timer = 0;
}

