#include "ScanState.h"
#include <Wheelson.h>
#include "WiFiConState.h"

Pair::ScanState::ScanState(Pair::PairService *pairService) : State(pairService){

}

Pair::ScanState::~ScanState() {

}

void Pair::ScanState::onStart() {
    cam = new Camera();
    testSprite = Wheelson.getDisplay().getBaseSprite();
    LoopManager::addListener(this);
}

void Pair::ScanState::onStop() {
    delete cam;
    cam = nullptr;
	testSprite = nullptr;
    LoopManager::removeListener(this);
}

void Pair::ScanState::loop(uint micros) {
    cam->loadFrame();
    testSprite->drawIcon(cam->getRGB565(),0,0,160,120);
    // TODO: Make this yourself, don't use other people's code
    std::vector<Aruco::Marker> markers = Markers::detect((uint8_t*)cam->getRGB565(), 160, 120, Markers::RGB565,
                                                         static_cast<Color *>(testSprite->frameBuffer(0)));
    cam->releaseFrame();
    if(!markers.empty()){
        pairService->setState(new Pair::WiFiConState(pairService, (uint16_t)markers[0].id));
    }

}
