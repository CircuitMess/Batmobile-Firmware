#include "WiFiConState.h"
#include <cstdio>
#include <Loop/LoopManager.h>
#include <WiFi.h>
#include "StreamConState.h"
#include <NetworkConfig.h>



Pair::WiFiConState::WiFiConState(Pair::PairService *pairService, uint16_t id) : State(pairService) {
	memcpy(ssid, "Batmobile ", 10);
	ssid[10] = (id / 100) + '0';
	ssid[11] = ((id / 10) % 10) + '0';
	ssid[12] = (id % 10) + '0';
	ssid[13] = '\0';

	memset(password, 0, 10);
	String batmobile = "Batmobile";
	for(int i = 0; i < 9; i++){
		char temp =  batmobile[i];
		temp = temp + id * 5 + 16;
		temp = temp % ('z' - 'A') + 'A';
		password[i] = temp;
	}

	WiFi.mode(WIFI_STA);
	WiFi.config(batmobileIP, gateway, subnet);
	startConnection();
	LoopManager::addListener(this);
}

Pair::WiFiConState::~WiFiConState() {

}

void Pair::WiFiConState::onStart() {

}

void Pair::WiFiConState::onStop() {
    LoopManager::removeListener(this);
}

void Pair::WiFiConState::loop(uint micros) {
    timeCounter += micros;
    if(timeCounter >= checkInterval){
        timeCounter -= checkInterval;
        if(WiFi.status() == WL_CONNECTED){
            Serial.println(WiFi.gatewayIP());
            pairService->setState(new StreamConState(pairService));
			return;
        }

        connectionTries++;
        if(connectionTries == 10){
            connectionTries = 0;
            WiFi.disconnect(true, true);
            startConnection();
        }
    }
}

void Pair::WiFiConState::startConnection() {
    WiFi.begin(ssid, password);
}
