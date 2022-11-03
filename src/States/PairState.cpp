#include "PairState.h"

PairState::PairState(){

}

PairState::~PairState(){

}

void PairState::onStart(){
	pair.setDoneCallback([](){
		Serial.printf("Paired\n"); // TODO: remove before release
		State::idleState();
	});

	pair.start();
}

void PairState::onStop(){
	pair.stop();
}
