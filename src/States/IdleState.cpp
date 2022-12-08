#include "IdleState.h"
#include <Batmobile.h>

IdleState::IdleState(){

}

IdleState::~IdleState(){

}

void IdleState::onStart(){
	Underlights.breathe({255, 0, 0}, {255, 122, 0}, 2000);
}

void IdleState::onStop(){
	Underlights.clear();
}
