#ifndef BATMOBILE_FIRMWARE_SCANSTATE_H
#define BATMOBILE_FIRMWARE_SCANSTATE_H

#include "PairService.h"
#include "State.h"
#include <Loop/LoopListener.h>
#include <Camera.h>
#include <Display/Sprite.h>
#include <Markers.h>

class Pair::ScanState : public Pair::State, private LoopListener{
public:
    ScanState(Pair::PairService *pairService);
    ~ScanState();
    void loop(uint micros) override;
protected:
    void onStart() override;
    void onStop() override;
private:
    Camera *cam;
    Sprite* testSprite;
    Aruco::Marker marker;
};


#endif //BATMOBILE_FIRMWARE_SCANSTATE_H