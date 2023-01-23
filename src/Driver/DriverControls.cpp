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
    Audio.play(SPIFFS.open("/SFX/honk.aac"));

    Taillights.setSolid(0);
    Headlights.setSolid(0);
    Underlights.setSolid({0,0,0});

	switch(lightsSwitch){
        case 0:
            break;
        case 1:
            Taillights.setSolid(255);
            Headlights.setSolid(255);
            break;
        case 2:
            Underlights.setSolid({255,0,0});
            break;
        case 3:
            Underlights.setSolid({0,255,0});
            break;
        case 4:
            Underlights.setSolid({0,0,255});
            break;
    }

    if(lightsSwitch == 4){
        lightsSwitch = 0;
    }else{
        lightsSwitch++;
    }
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
