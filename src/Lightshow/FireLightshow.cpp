#include "FireLightshow.h"

void FireLightshow::onStart(){
	LoopManager::addListener(this);
	Headlights.setSolid(255);
}

void FireLightshow::onStop(){
	LoopManager::removeListener(this);
}

void FireLightshow::loop(uint micros){
	fireTimer += micros;
	if(fireTimer >= fireRandomDuration){
		fireTimer = 0;
		fireRandomDuration = random(100000);
		Taillights.setSolid(random(200) + 40);
	}
}
