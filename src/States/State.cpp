#include "State.h"

State::State(){

}

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

void State::onStart(){
	currentState = this;
}

void State::onStop(){

}
