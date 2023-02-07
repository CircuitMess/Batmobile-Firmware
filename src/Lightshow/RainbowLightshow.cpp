#include "RainbowLightshow.h"
#include <ColorConverter.h>
#include <Loop/LoopManager.h>


void RainbowLightshow::onStart(){
	LoopManager::addListener(this);
}

void RainbowLightshow::onStop(){
	LoopManager::removeListener(this);
}

void RainbowLightshow::loop(uint micros){
	timer = (timer + micros) % fadeTime;
	uint8_t hue = (timer / fadeTime) * 180.0;
	Serial.println(hue);
	Underlights.setSolid(ColorConverter::toRGB(hue));
}
