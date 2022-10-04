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
    String ssidString = "Batmobile ";
    ssidString += String(id);
    ssid = ssidString.c_str();
    Serial.printf("\nconnecting to: %s", ssid);

    memset(password, 0, 10);
    String batmobile = "Batmobile";
    for(int i = 0; i < 9; i++){
        char temp =  batmobile[i];
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
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
}
