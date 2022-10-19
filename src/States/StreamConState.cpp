#include <Loop/LoopManager.h>
#include "StreamConState.h"
#include <AsyncTCP.h>
#include "ScanState.h"
#include <Communication/Communication.h>
#include <NetworkConfig.h>
#include "DoneState.h"

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
        Serial.printf("Second passed: ");
        if(client->connected()){
            pairService->setState(new DoneState(pairService));
        }else{
            Serial.println("Client connecting");
            if(client->connect(controllerIP, port)) Serial.printf("onConnect\n");
        }
        client->add(dummyData,sizeof(dummyData));
        if(client->canSend()){
            client->send();
            Serial.println("Data sent");
        }else{
            Serial.printf("non va piu\n");
        }
    }

}


