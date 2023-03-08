#include "IdleState.h"
#include <Batmobile.h>
#include "../SFXPaths.h"

IdleState::IdleState(){

}

IdleState::~IdleState(){

}

void IdleState::onStart(){
	Underlights.breathe({ 0, 255, 50 }, { 120, 0, 255 }, 6000);
	LoopManager::addListener(this);
	Com.addListener({ ComType::SettingsSound, ComType::IdleSounds, ComType::SoundEffect }, this);
	srand(time(NULL));
	threshold = (rand() % 11 + 10) * 1000000; //10-20 sec
}

void IdleState::onStop(){
	Com.removeListener(this);
	LoopManager::removeListener(this);
	Underlights.clear();
}

void IdleState::onSettingsSound(){
	Audio.play(SPIFFS.open("/SFX/beep.aac"));
}

void IdleState::loop(uint micros){
	if(!soundsOn) return;
	counter += micros;
	if(counter >= threshold && !Audio.isPlaying()){
		counter = 0;
		uint8_t honkRand = rand() % 5;
		if(honkRand == 0){
			Audio.play(SPIFFS.open("/SFX/bats.aac"));
		}else{
			Audio.play(SPIFFS.open(String("/SFX/idle") + honkRand + ".aac"));
		}
		threshold = (rand() % 11 + 10) * 1000000; //10-20 sec
	}
}

void IdleState::onIdleSounds(bool toggle){
	soundsOn = toggle;
	if(!toggle){
		counter = 0;
	}
}

void IdleState::onSoundEffect(uint8_t sample){
	if(sample >= SFXPaths.size()) return;

	File f = SPIFFS.open(SFXPaths[sample]);
	if(!f) return;

	Audio.play(f);
}
