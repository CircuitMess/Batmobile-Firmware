
#include "PoliceLightshow.h"

void PoliceLightshow::onStart(){
	LoopManager::addListener(this);
	Underlights.setSolid({255,0,0});
}

void PoliceLightshow::onStop(){
	LoopManager::removeListener(this);
}

void PoliceLightshow::loop(uint micros){
	counter += micros;

	if(counter >= BlinkInterval){
		counter = 0;
		if(blue){
			Underlights.setSolid({255,0,0});
		}else{
			Underlights.setSolid({0,0,255});
		}
		blue = !blue;
	}

}
