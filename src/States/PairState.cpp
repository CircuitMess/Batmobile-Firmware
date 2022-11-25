#include "PairState.h"
#include <Batmobile.h>

PairState::PairState(){

}

PairState::~PairState(){

}

void PairState::onStart(){
	pair.start();
	Underlights.breathe({ 0, 0, 0 }, { 255, 0, 0 }, 2000);
}

void PairState::onStop(){
	pair.stop();
}
