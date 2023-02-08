#include <Communication/Communication.h>
#include <Batmobile.h>
#include "DriverControls.h"

void DriverControls::start(){
	Com.addListener({ ComType::Honk, ComType::DriveSpeed }, this);
	srand (time(NULL));
}

void DriverControls::stop(){
	Com.removeListener(this);
}

void DriverControls::onHonk(){
	uint8_t honkRand = rand() % 6;
    Audio.play(SPIFFS.open(String("/SFX/honk") + honkRand + ".aac"));
	Serial.println(honkRand);
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
