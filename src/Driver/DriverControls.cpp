#include <Communication/Communication.h>
#include <Batmobile.h>
#include "DriverControls.h"

void DriverControls::start(){
	Com.addListener({ ComType::Honk, ComType::DriveSpeed, ComType::Boost }, this);
	prevType = LightshowType::None;
	lightshowType = LightshowType::None;
}

void DriverControls::stop(){
	Com.removeListener(this);
	if(lightshowDisable) return;
	if(lightshow) lightshow->stop();
}

void DriverControls::onHonk(){
	Audio.play(SPIFFS.open(String("/SFX/honk") + (esp_random()%7) + ".aac"));

	if(boosting) return;

	if(lightshowDisable) return;

	if(lightshow) lightshow->stop();
	lightshowType = (LightshowType) ((uint8_t) (((uint8_t) lightshowType) + 1) % (uint8_t) LightshowType::Size);
	lightshow = Lightshow::createLightshow(lightshowType);
	if(lightshow) lightshow->start();
}

void DriverControls::onDriveSpeed(uint8_t speed){
	driveSpeed = speed;
}

uint8_t DriverControls::getDriveSpeed() const{
	return driveSpeed;
}

void DriverControls::onBoost(bool boost){
	if(boost == boosting) return;
	boosting = boost;

	if(lightshowDisable) return;

	if(boost && lightshowType != LightshowType::FrontAndFire){
		prevType = lightshowType;
		lightshowType = LightshowType::FrontAndFire;
		lightshow = Lightshow::createLightshow(LightshowType::FrontAndFire);
		if(lightshow) lightshow->start();
	}else if(boost && lightshowType == LightshowType::FrontAndFire){
		prevType = LightshowType::FrontAndFire;
	}else if(!boost && lightshowType == LightshowType::FrontAndFire && prevType != LightshowType::FrontAndFire){
		lightshowType = prevType;
		lightshow = Lightshow::createLightshow(lightshowType);
		if(lightshow) lightshow->start();
	}
}



void DriverControls::setLightshowDisable(bool lightshowDisable){
	DriverControls::lightshowDisable = lightshowDisable;
}
