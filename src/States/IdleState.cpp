#include "IdleState.h"
#include <Batmobile.h>

IdleState::IdleState(){

}

IdleState::~IdleState(){

}

void IdleState::onStart(){
	Underlights.breathe({0, 255, 50}, {120, 0, 255}, 6000);

	Com.addListener({ComType::SettingsSound, ComType::Volume}, this);
}

void IdleState::onStop(){
	Com.removeListener(this);
	Underlights.clear();
}

void IdleState::onSettingsSound(){
	Audio.play(SPIFFS.open("/SFX/driverExit.aac"));
}

void IdleState::onVolume(uint8_t volume){
	Audio.setVolume(constrain(volume, 0, 100));
}
