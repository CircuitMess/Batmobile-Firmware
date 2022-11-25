#include <Communication/Communication.h>
#include <Batmobile.h>
#include "DriverControls.h"

void DriverControls::start(){
	Com.addListener({ ComType::Honk, ComType::DriveSpeed }, this);
}

void DriverControls::stop(){
	Com.removeListener(this);
}

void DriverControls::onHonk(){
	headlightsToggle = !headlightsToggle;
	Taillights.setSolid(headlightsToggle ? 255 : 0);
	Headlights.setSolid(headlightsToggle ? 255 : 0);
}

bool DriverControls::getHeadlightsToggle() const{
	return headlightsToggle;
}

void DriverControls::onDriveSpeed(uint8_t speed){
	driveSpeed = speed;
}

uint8_t DriverControls::getDriveSpeed() const{
	return driveSpeed;
}
