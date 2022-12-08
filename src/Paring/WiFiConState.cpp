#include "WiFiConState.h"
#include <cstdio>
#include <Loop/LoopManager.h>
#include <WiFi.h>
#include "StreamConState.h"
#include <NetworkConfig.h>
#include <Batmobile.h>
#include "ScanState.h"

Pair::WiFiConState::WiFiConState(Pair::PairService* pairService, uint16_t id) : State(pairService) {
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
}

void Pair::WiFiConState::onStart() {

	Underlights.blinkContinuous({ 255, 0, 0 }, 100);

	retryCounter = 0;
	retryCount = 0;

	startConnection();
	LoopManager::addListener(this);
}

void Pair::WiFiConState::onStop() {
    LoopManager::removeListener(this);
	Underlights.clear();
}

void Pair::WiFiConState::loop(uint micros) {
	if(WiFi.status() == WL_CONNECTED){
		pairService->setState(new StreamConState(pairService));
		return;
	}

	retryCounter += micros;
    if(retryCounter >= RetryInterval){
		retryCounter = 0;
        retryCount++;

        if(retryCount >= RetryTries){
			pairService->setState(new ScanState(pairService));
			return;
        }

		WiFi.disconnect(true, true);
		startConnection();
    }

	delay(10);
}

void Pair::WiFiConState::startConnection() {
	WiFi.begin(ssid, password);
	delay(100);
	WiFi.config(batmobileIP, gateway, subnet);
}
