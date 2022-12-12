#include "ScanState.h"
#include <Batmobile.h>
#include "WiFiConState.h"
#include <Loop/LoopManager.h>

Pair::ScanState::ScanState(Pair::PairService *pairService) : State(pairService){

}

void Pair::ScanState::onStart(){
	S3.setMode(DriveMode::Marker);
    LoopManager::addListener(this);
}

void Pair::ScanState::onStop(){
	S3.setMode(DriveMode::Idle);
    LoopManager::removeListener(this);
}

void Pair::ScanState::loop(uint micros){
	// TODO: Use S3 frames for marker detection

	// TODO: place this after scan
	Audio.play(SPIFFS.open("/SFX/scan.aac"));

/*	cam->loadFrame();
    std::vector<Aruco::Marker> markers = Markers::detect((uint8_t*)cam->getRGB565(), 160, 120, Markers::RGB565);
    cam->releaseFrame();

    if(!markers.empty()){
        pairService->setState(new Pair::WiFiConState(pairService, (uint16_t) markers[0].id));
    }*/

}
