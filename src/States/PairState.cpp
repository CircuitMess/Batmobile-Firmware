#include "PairState.h"
#include "StateManager.h"
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
	Underlights.clear();
}
