#include "IdleState.h"
#include <Batmobile.h>

IdleState::IdleState(){

}

IdleState::~IdleState(){

}

void IdleState::onStart(){
	Underlights.breathe({0, 255, 50}, {120, 0, 255}, 6000);
}

void IdleState::onStop(){
	Underlights.clear();
}
