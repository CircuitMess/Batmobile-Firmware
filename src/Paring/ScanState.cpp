#include "ScanState.h"
#include <Batmobile.h>
#include "WiFiConState.h"
#include "../States/StateManager.h"
#include <Loop/LoopManager.h>
#include <NetworkConfig.h>

Pair::ScanState::ScanState(Pair::PairService* pairService) : State(pairService){

}

void Pair::ScanState::onStart(){
	S3.setMode(DriveMode::QRScan);
	LoopManager::addListener(this);
	Underlights.breathe({ 50, 0, 0 }, { 255, 0, 0 }, 2000);
	timeoutCounter = 0;
}

void Pair::ScanState::onStop(){
	S3.setMode(DriveMode::Idle);
	LoopManager::removeListener(this);
	Underlights.clear();
}

void Pair::ScanState::loop(uint micros){
	timeoutCounter += micros;
	if(timeoutCounter >= pairTimeout){
		Batmobile.shutdownNotify();
		return;
	}

	auto frame = S3.getFrame();
	if(frame == nullptr || frame->toQR() == nullptr) return;

	auto markers = frame->toQR();

	if(!markers->arucoMarkers.empty()){
		printf("Aruco %d\n", markers->arucoMarkers.front().id );
		Audio.play(SPIFFS.open("/SFX/scan.aac"));
		delay(300);
		arucoFound(markers->arucoMarkers.front().id);
	}else if(!markers->qrMarkers.empty()){
		printf("QR %s\n", markers->qrMarkers.front().data);
		Audio.play(SPIFFS.open("/SFX/scan.aac"));
		delay(300);
		qrFound((char*) markers->qrMarkers.front().data);
	}
}

void Pair::ScanState::arucoFound(uint16_t id){
	char ssid[14];
	char pass[10];

	memcpy(ssid, "Batmobile ", 10);
	ssid[10] = (id / 100) + '0';
	ssid[11] = ((id / 10) % 10) + '0';
	ssid[12] = (id % 10) + '0';
	ssid[13] = '\0';

	memset(pass, 0, 10);
	String batmobile = "Batmobile";
	for(int i = 0; i < 9; i++){
		char temp = batmobile[i];
		temp = temp + id * 5 + 16;
		temp = temp % ('z' - 'A') + 'A';
		pass[i] = temp;
	}

	controllerIP = defaultControllerIP;

	pairService->setState(new Pair::WiFiConState(pairService, ssid, pass));
}

void Pair::ScanState::qrFound(const char* data){
	/*
	 * QR data is consisted of:
	 * 24 bytes for SSID + 1 null terminator
	 * 23 bytes for pass + 1 null terminator
	 * 4 bytes for IP address of controller
	 *
	 * Total of 53 bytes
	 */

	for(int i = 0; i < 4; i++){
		controllerIP[i] = data[49 + i];
	}

	pairService->setState(new Pair::WiFiConState(pairService, data, data + 25));
}
