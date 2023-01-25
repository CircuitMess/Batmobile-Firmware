#include <Batmobile.h>
#include "BallDriver.h"

BallDriver::BallDriver() : Driver(DriveMode::Ball){
	Com.addListener(ComType::BallHue, this);
}

BallDriver::~BallDriver(){
	Com.removeListener(this);
}

void BallDriver::onFrame(DriveInfo& driveInfo){

}

void BallDriver::onBallHue(uint8_t hue){
	S3.setHue(hue);
}
