#include <Loop/LoopManager.h>
#include "StreamConState.h"
#include <AsyncTCP.h>
#include "ScanState.h"
#include <NetworkConfig.h>
#include "DoneState.h"

Pair::StreamConState::StreamConState(Pair::PairService *pairService) : State(pairService) {
    client = new AsyncClient;
}

Pair::StreamConState::~StreamConState() {

}


void Pair::StreamConState::onStart() {
    client->onData([this](void* arg, AsyncClient* server, void* data, size_t len){
        Serial.printf("\n dummyData received from %s \n", client->remoteIP().toString().c_str());
        Serial.write((uint8_t *)data, len);
    }, client);

    client->onConnect([this](void*, AsyncClient* server){
        Serial.printf("PAIRED!\n");
//        pairService->doneCallback(client);
//        pairService->setState(nullptr);
    }, nullptr);

    client->onError([this](void*, AsyncClient* c, int8_t error){
        Serial.print("error occurred: ");
        Serial.println(c->errorToString(error));
    }, nullptr);

    client->setAckTimeout(10000000);

    client->onTimeout([this](void*, AsyncClient* c, uint32_t time){
        pairService->setState(new ScanState(pairService));
        Serial.printf("timeout occurred\n");
    });
    Serial.printf("\nRX: %d\t ACK: %d\n",client->getRxTimeout(), client->getAckTimeout());
    client->onConnect([this](void*, AsyncClient* client){
        Serial.printf("Connected\n");
        client->add(dummyData,sizeof(dummyData));
        while(!client->canSend()){
            Serial.printf("non va piu\n");
        }
        Serial.printf("B\n");
        client->send();
        Serial.printf("C\n");
    });


    if(client->connect(controllerIP, port)) Serial.printf("YASS\n");


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


