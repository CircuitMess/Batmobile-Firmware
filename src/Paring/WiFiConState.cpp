#include "WiFiConState.h"
#include <cstdio>
#include <Loop/LoopManager.h>
#include <WiFi.h>
#include "StreamConState.h"
#include <NetworkConfig.h>
#include <Batmobile.h>
#include "ScanState.h"
#include <Batmobile.h>

Pair::WiFiConState::WiFiConState(Pair::PairService* pairService, std::string ssid, std::string pass) : State(pairService), ssid(std::move(ssid)),
																									   pass(std::move(pass)){

}

void Pair::WiFiConState::onStart(){
	Underlights.blinkContinuous({ 255, 0, 0 }, 100);

	retryCounter = 0;
	retryCount = 0;

	startConnection();
	LoopManager::addListener(this);
}

void Pair::WiFiConState::onStop(){
	LoopManager::removeListener(this);
}

void Pair::WiFiConState::loop(uint micros){
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

void Pair::WiFiConState::startConnection(){
	WiFi.begin(ssid.c_str(), pass.c_str());
	delay(100);
//	WiFi.config(batmobileIP, gateway, subnet);
}
