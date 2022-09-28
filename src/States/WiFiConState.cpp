#include "WiFiConState.h"
#include <cstdio>
#include <Loop/LoopManager.h>
#include "WiFi.h"

Pair::WiFiConState::WiFiConState(Pair::PairService *pairService, uint16_t id) : State(pairService) {
    char numChar[3];
    std::sprintf(numChar, "%d", id);
    ssid += numChar;
    Serial.printf("\nconnecting to: %s", ssid.c_str());

    std::string batmobile = "Batmobile";
    int i = 0;
    for(char& c: batmobile){
        int temp = (int) c;
        temp = temp + id * 5 + 16;
        temp = temp % (121 - 64) + 64;
        password[i] = (char) temp;
        i++;
    }
    Serial.printf("\nPass: %s\n",password);
}

Pair::WiFiConState::~WiFiConState() {

}

void Pair::WiFiConState::onStart() {
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
            Serial.println(WiFi.localIP());
            LoopManager::removeListener(this);
            //TODO: Switch to the StreamConState
        }
    }
}

void Pair::WiFiConState::startConnection() {
    WiFi.begin(ssid.c_str(), password);
    Serial.print("Connecting to WiFi ..");
}
