#include "WiFiConState.h"
#include <cstdio>
#include <Loop/LoopManager.h>
#include "WiFi.h"
#include "StreamConState.h"


Pair::WiFiConState::WiFiConState(Pair::PairService *pairService, uint16_t id) : State(pairService), id(id) {

}

Pair::WiFiConState::~WiFiConState() {

}

void Pair::WiFiConState::onStart() {
    char numChar[3];
    std::sprintf(numChar, "%d", id);
    ssid += numChar;
    Serial.printf("\nconnecting to: %s", ssid.c_str());

    char batmobile[8];
    strncpy(batmobile,"Batmobil", sizeof(batmobile));
    for(int i = 0; i < 8; i++){
        char temp = batmobile[i];
        temp = temp + id * 5 + 16;
        temp = temp % ('z' - 'A') + 'A';
        password[i] = temp;
    }
    Serial.printf("\nPass: %s\n",password);

    WiFi.mode(WIFI_STA);
    startConnection();
    LoopManager::addListener(this);
}

void Pair::WiFiConState::onStop() {
    LoopManager::removeListener(this);
}

void Pair::WiFiConState::loop(uint micros) {
    timeCounter += micros;
    if(timeCounter >= 1000000){
        timeCounter -= 1000000;
        secondCounter++;
        Serial.print('.');
        if(secondCounter == 5){
            secondCounter = 0;
            startConnection();
        }
        if(WiFi.status() == WL_CONNECTED){
            Serial.println(WiFi.gatewayIP());
            LoopManager::removeListener(this);
            pairService->setState(new StreamConState(pairService));
        }
    }
}

void Pair::WiFiConState::startConnection() {
    WiFi.begin(ssid.c_str(), password);
    Serial.print("Connecting to WiFi ..");
}
