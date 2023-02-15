#include <Communication/Communication.h>
#include <Batmobile.h>
#include "DriverControls.h"
#include "../Lightshow/BreatheLightshow.h"
#include "../Lightshow/RainbowLightshow.h"
#include "../Lightshow/FireLightshow.h"
#include "../Lightshow/SolidLightshow.h"

void DriverControls::start(){
	Com.addListener({ ComType::Honk, ComType::DriveSpeed }, this);
	prevType = LightshowType::None;
	lightshowType = LightshowType::None;
}

void DriverControls::stop(){
	Com.removeListener(this);
	if(lightshowDisable) return;
	if(lightshow) lightshow->stop();
}

void DriverControls::onHonk(){
	Audio.play(SPIFFS.open("/SFX/honk.aac"));

	if(boosting) return;

	if(lightshowDisable) return;

	if(lightshow) lightshow->stop();
	lightshowType = (LightshowType) ((uint8_t) (((uint8_t) lightshowType) + 1) % (uint8_t) LightshowType::Size);
	lightshow = createLightshow(lightshowType);
	if(lightshow) lightshow->start();
}

void DriverControls::onDriveSpeed(uint8_t speed){
	driveSpeed = speed;
}

uint8_t DriverControls::getDriveSpeed() const{
	return driveSpeed;
}

void DriverControls::onBoost(bool boost){
	boosting = boost;

	if(lightshowDisable) return;

	if(boost && lightshowType != LightshowType::FrontAndFire){
		prevType = lightshowType;
		lightshowType = LightshowType::FrontAndFire;
		lightshow = createLightshow(LightshowType::FrontAndFire);
		if(lightshow) lightshow->start();
	}else if(!boost && lightshowType == LightshowType::FrontAndFire && prevType != LightshowType::FrontAndFire){
		lightshowType = prevType;
		lightshow = createLightshow(lightshowType);
		if(lightshow) lightshow->start();
	}
}

std::unique_ptr<Lightshow> DriverControls::createLightshow(LightshowType type){
	switch(type){
		case LightshowType::SolidFrontBack:
			return std::make_unique<SolidLightshow>();
			break;
		case LightshowType::BreatheR:
			return std::make_unique<BreatheLightshow<LightshowType::BreatheR>>();
			break;
		case LightshowType::BreatheG:
			return std::make_unique<BreatheLightshow<LightshowType::BreatheG>>();
			break;
		case LightshowType::BreatheB:
			return std::make_unique<BreatheLightshow<LightshowType::BreatheB>>();
			break;
		case LightshowType::Rainbow:
			return std::make_unique<RainbowLightshow>();
			break;
		case LightshowType::BreatheRFrontBack:
			return std::make_unique<BreatheLightshow<LightshowType::BreatheRFrontBack>>();
			break;
		case LightshowType::FrontAndFire:
			return std::make_unique<FireLightshow>();
			break;
		default:
			return nullptr;
			break;
	}
}

void DriverControls::setLightshowDisable(bool lightshowDisable){
	DriverControls::lightshowDisable = lightshowDisable;
}
