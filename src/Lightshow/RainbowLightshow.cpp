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
	uint8_t hue = ((float)timer / (float)fadeTime) * 180.0;
	auto c = ColorConverter::toRGB(hue);
	Underlights.setSolid(c);
}
