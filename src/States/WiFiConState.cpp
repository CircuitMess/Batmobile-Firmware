#include "WiFiConState.h"
#include <cstdio>
#include "WiFi.h"

Pair::WiFiConState::WiFiConState(Pair::PairService *pairService, uint16_t id) : State(pairService), id(id) {
    char numChar[3];
    std::sprintf(numChar, "%d", id);
    ssid += numChar;
    Serial.printf("\nconnecting to: %s", ssid.c_str());

    int i = 0;
    for(char& c: batmobile){
        int temp = (int) c;
        temp = temp + id * 5 + 16;
        temp = temp % (121 - 64) + 64;
        password[i] = (char) temp;
        i++;
    }
}

Pair::WiFiConState::~WiFiConState() {

}

void Pair::WiFiConState::loop(uint micros) {

}

void Pair::WiFiConState::onStart() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

void Pair::WiFiConState::onStop() {

}
