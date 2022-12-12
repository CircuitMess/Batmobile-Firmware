#include <Communication/Communication.h>
#include <Batmobile.h>
#include "StateManager.h"
#include "IdleState.h"
#include "DriveState.h"
#include "PairState.h"

StateManager::StateManager(){
	Com.addDcListener(this);
	Com.addListener(ComType::DriveMode, this);

	S3.setMode(DriveMode::Idle);
	currentMode = DriveMode::Idle;
}

void StateManager::begin(){
	currentState = std::make_unique<PairState>();
	currentState->start();
}

StateManager::~StateManager(){
	Com.removeDcListener(this);
	Com.removeListener(this);
}

void StateManager::onConnected(){
	if(currentState){
		currentState->stop();
		currentState.reset();
	}

	Audio.play(SPIFFS.open("/SFX/paired.aac"));

	/** Controller could immediately send a setState packet. Make sure there isn't a race condition between the resulting state change
	 * and the idleState here. TODO */

	S3.setMode(DriveMode::Idle);
	currentMode = DriveMode::Idle;
	currentState = std::make_unique<IdleState>();
	currentState->start();
}

void StateManager::onDisconnected(){
	Audio.play(SPIFFS.open("/SFX/disconnect.aac"));

	if(currentState){
		currentState->stop();
		currentState.reset();
	}

	S3.setMode(DriveMode::Idle);
	currentMode = DriveMode::Idle;
	currentState = std::make_unique<PairState>();
	currentState->start();
}

void StateManager::onDriveMode(DriveMode mode){
	if(mode == currentMode) return;

	if(currentState){
		currentState->stop();
		currentState.reset();
	}

	S3.setMode(mode);
	currentMode = mode;

	if(mode == DriveMode::Idle){
		currentState = std::make_unique<IdleState>();
	}else{
		currentState = std::make_unique<DriveState>(mode);
	}

	if(currentState){
		currentState->start();
	}
}
