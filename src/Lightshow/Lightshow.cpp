#include "Lightshow.h"
#include "SolidLightshow.h"
#include "BreatheLightshow.h"
#include "RainbowLightshow.h"
#include "FireLightshow.h"
#include "PoliceLightshow.h"
#include <Batmobile.h>

Lightshow::~Lightshow(){
	stop();
}

void Lightshow::start(){
	onStart();
}

void Lightshow::stop(){
	onStop();

	Headlights.clear();
	Taillights.clear();
	Underlights.clear();
}

std::unique_ptr<Lightshow> Lightshow::createLightshow(LightshowType type){
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
		case LightshowType::Police:
			return std::make_unique<PoliceLightshow>();
			break;
		default:
			return nullptr;
			break;
	}
}
