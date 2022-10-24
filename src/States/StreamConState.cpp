#include <Loop/LoopManager.h>
#include "StreamConState.h"
#include <AsyncTCP.h>
#include "ScanState.h"
#include <Communication/Communication.h>
#include <NetworkConfig.h>

Pair::StreamConState::StreamConState(Pair::PairService *pairService) : State(pairService) {
    client = new AsyncClient;
}

Pair::StreamConState::~StreamConState() {

}


void Pair::StreamConState::onStart() {
    Com.setClient(client);
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

        }else{
            client->connect(controllerIP, port);
            connectTries++;
        }
        if(connectTries == 5){
            pairService->setState(new ScanState(pairService));
        }
    }

}


