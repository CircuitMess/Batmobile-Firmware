#include "PairService.h"
#include "State.h"
#include "ScanState.h"
#include <Communication/Communication.h>

Pair::PairService::PairService(){ }

Pair::PairService::~PairService(){
	stop();
}

void Pair::PairService::start(){
	if(currentState) return;

	currentState = new Pair::ScanState(this);
	currentState->start();
}

void Pair::PairService::stop(){
	if(currentState){
		delete currentState;
	}
	currentState = nullptr;
}

void Pair::PairService::setState(Pair::State* state){
	if(currentState != nullptr){
		currentState->stop();
	}
    delete currentState;

    currentState = state;
    currentState->start();
}

void Pair::PairService::paringDone(std::unique_ptr<AsyncClient> client){
	if(currentState){
		currentState->stop();
	}
	delete currentState;
	currentState = nullptr;

	/** TODO: Potential hazard
	 * Com.setClient calls connection state listeners -> StateManager deletes PairState, which in turn deletes the PairService.
	 * doneCallback is called when PairService no longer exists, and probably which ever object owned the PairService too. */
	Com.setClient(std::move(client));
	if(doneCallback){
		doneCallback();
	}
}

void Pair::PairService::setDoneCallback(std::function<void()> callback){
	this->doneCallback = std::move(callback);
}
