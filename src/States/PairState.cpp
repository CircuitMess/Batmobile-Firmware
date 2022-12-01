#include "PairState.h"
#include <Batmobile.h>

PairState::PairState(){

}

PairState::~PairState(){

}

void PairState::onStart(){
	pair.start();
}

void PairState::onStop(){
	pair.stop();
	Audio.play(SPIFFS.open("/SFX/paired.aac"));
}
