#include "ScanState.h"
#include <Wheelson.h>
#include "WiFiConState.h"

Pair::ScanState::ScanState(Pair::PairService *pairService) : State(pairService){

}

void Pair::ScanState::onStart(){
    cam = new Camera();
    LoopManager::addListener(this);
}

void Pair::ScanState::onStop(){
    delete cam;
    cam = nullptr;
    LoopManager::removeListener(this);
}

void Pair::ScanState::loop(uint micros){
	auto* drawBuffer = static_cast<Color*>(Wheelson.getDisplay().getBaseSprite()->frameBuffer(0));

	cam->loadFrame();
    std::vector<Aruco::Marker> markers = Markers::detect((uint8_t*)cam->getRGB565(), 160, 120, Markers::RGB565);
    cam->releaseFrame();

    if(!markers.empty()){
        pairService->setState(new Pair::WiFiConState(pairService, (uint16_t) markers[0].id));
    }

}
