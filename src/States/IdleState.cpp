#include <Batmobile.h>
#include "IdleState.h"

IdleState::IdleState(){

}

IdleState::~IdleState(){

}

void IdleState::onStart(){
	Headlights.clear();
	Taillights.clear();
	Underlights.breathe({255, 0, 0}, {255, 122, 0}, 2000);
}

void IdleState::onStop(){
}
