#include <Loop/LoopManager.h>
#include "StreamConState.h"
#include <AsyncTCP.h>
#include "ScanState.h"
#include <NetworkConfig.h>
#include <Batmobile.h>

Pair::StreamConState::StreamConState(Pair::PairService *pairService) : State(pairService){

}

void Pair::StreamConState::onStart(){
	client = std::make_unique<AsyncClient>();
    client->connect(controllerIP, controlPort);

	LoopManager::addListener(this);
	timeoutCounter = 0;
}

void Pair::StreamConState::onStop(){
	Underlights.clear();

    LoopManager::removeListener(this);

	client.reset();
}

void Pair::StreamConState::loop(uint micros){
	timeoutCounter += micros;
	if(timeoutCounter >= ConTimeout){
		Audio.play(SPIFFS.open("/SFX/disconnect.aac"));
		pairService->setState(new ScanState(pairService));
		return;
	}
	if(!client || !client->connected()) return;

	pairService->paringDone(std::move(client));
}
