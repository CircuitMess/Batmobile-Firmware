#include "PairState.h"

PairState::PairState(){

}

PairState::~PairState(){

}

void PairState::onStart(){
	pair.start();
}

void PairState::onStop(){
	pair.stop();
}
