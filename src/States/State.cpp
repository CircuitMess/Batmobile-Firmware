#include "State.h"
#include "DriveState.h"
#include "PairState.h"
#include "IdleState.h"

State* State::currentState = nullptr;

void State::start(){
	if(started) return;
	started = true;
	onStart();
}

void State::stop(){
	if(!started) return;
	started = false;
	onStop();
}

void State::pairState(){
	if(currentState != nullptr){
		currentState->stop();
		delete currentState;
	}
	currentState = new PairState();
	currentState->start();
}

void State::driveState(){
	if(currentState != nullptr){
		currentState->stop();
		delete currentState;
	}
	currentState = new DriveState();
	currentState->start();

}

void State::idleState(){
	if(currentState != nullptr){
		currentState->stop();
		delete currentState;
	}
	currentState = new IdleState();
	currentState->start();

}