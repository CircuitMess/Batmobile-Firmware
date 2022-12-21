#include "PairState.h"
#include "StateManager.h"
#include <Batmobile.h>

PairState::PairState(){

}

PairState::~PairState(){

}

void PairState::onStart(){
	pair.start();
	LoopManager::addListener(this);
}

void PairState::onStop(){
	LoopManager::removeListener(this);

	pair.stop();
	Underlights.clear();
}

void PairState::loop(uint micros){
	timeoutCounter += micros;
	if(timeoutCounter >= pairTimeout){
		StateManager::shutdown();
	}
}
