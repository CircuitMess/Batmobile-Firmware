#include <Loop/LoopManager.h>
#include "StreamConState.h"
#include <AsyncTCP.h>
#include "ScanState.h"
#include <NetworkConfig.h>
#include <Batmobile.h>

Pair::StreamConState::StreamConState(Pair::PairService *pairService) : State(pairService){

}

void Pair::StreamConState::onStart(){
	Underlights.blinkContinuous({ 255, 0, 0 }, 100);

	client = std::make_unique<AsyncClient>();
    client->connect(controllerIP, controlPort);

	LoopManager::addListener(this);
}

void Pair::StreamConState::onStop(){
	Underlights.clear();

    LoopManager::removeListener(this);

	client.reset();
}

void Pair::StreamConState::loop(uint micros){
	if(!client || !client->connected()) return;

	pairService->paringDone(std::move(client));
}
