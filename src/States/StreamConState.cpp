#include <Loop/LoopManager.h>
#include "StreamConState.h"
#include <AsyncTCP.h>
#include "ScanState.h"
#include <NetworkConfig.h>

Pair::StreamConState::StreamConState(Pair::PairService *pairService) : State(pairService), client(std::make_unique<AsyncClient>()) {
}

Pair::StreamConState::~StreamConState() {}

void Pair::StreamConState::onStart() {
    client->connect(controllerIP, port);
    delay(100);
    LoopManager::addListener(this);
}

void Pair::StreamConState::onStop() {
    LoopManager::removeListener(this);
}

void Pair::StreamConState::loop(uint micros) {
    time += micros;

    if(time >= 1000000){
        time -= 1000000;
        if(client->connected()){
			pairService->paringDone(std::move(client));
			return;
        }else{
            client->connect(controllerIP, port);
            connectTries++;
			if(connectTries == 5){
				pairService->setState(new ScanState(pairService));
			}
        }
    }

}


