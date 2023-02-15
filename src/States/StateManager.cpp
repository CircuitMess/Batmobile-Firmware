#include <Communication/Communication.h>
#include <Batmobile.h>
#include "StateManager.h"
#include "IdleState.h"
#include "DriveState.h"
#include "PairState.h"

StateManager::StateManager(){
	Com.addDcListener(this);
	Com.addListener({ComType::DriveMode, ComType::Disconnect, ComType::Volume, ComType::Shutdown, ComType::OverrideSound}, this);

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

	Underlights.clear();
	Underlights.blinkTwice({ 0, 255, 0 });

	const uint32_t m = millis();
	while(millis() - m <= 800){
		Underlights.loop(0);
	}

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

void StateManager::onDisconnectRequest(){
	Com.setClient(nullptr);
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

void StateManager::onVolume(uint8_t volume){
	Audio.setVolume(constrain((float) volume * 2.55f, 0.0f, 255.0f));
}

void StateManager::onShutdown(){
	Com.sendShutdownAck();
	delay(2000);
	Batmobile.shutdownNotify();
}

void StateManager::onOverrideSound(bool manual){
	Audio.play(SPIFFS.open( manual ? "/SFX/manualSlow.aac" : "/SFX/autopilotSlow.aac"));
}


