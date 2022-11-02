#include "PairService.h"
#include "State.h"
#include "ScanState.h"
#include <Communication/Communication.h>

Pair::PairService::PairService() {
    currentState = new Pair::ScanState(this);
    currentState->start();
}

Pair::PairService::~PairService(){
	currentState->stop();
	delete currentState;
}

void Pair::PairService::setState(Pair::State* state){
    currentState->stop();
    delete currentState;
    currentState = state;
    currentState->start();
}


void Pair::PairService::paringDone(std::unique_ptr<AsyncClient> client){
	currentState->stop();
	delete currentState;
	currentState = nullptr;
	Com.setClient(std::move(client));
	if(callback) callback();
}

void Pair::PairService::setDoneCallback(std::function<void()> callback){
	this->callback = callback;
}
