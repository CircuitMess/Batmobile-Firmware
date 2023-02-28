#include "BallDriver.h"
#include <Batmobile.h>
#include <ColorConverter.h>

BallDriver::BallDriver() : Driver(DriveMode::Ball){

}

BallDriver::~BallDriver(){
	stop();
}

void BallDriver::onStart(){
	Com.addListener({ComType::BallHue, ComType::MotorsTimeoutClear, ComType::MotorsTimeout}, this);
	S3.setHue(hue);
	Underlights.setSolid(ColorConverter::toRGB(hue));
}

void BallDriver::onStop(){
	Com.removeListener(this);
}

void BallDriver::onFrame(DriveInfo& driveInfo){
	if(driveInfo.mode != DriveMode::Ball) return;
	auto ballInfo = driveInfo.toBall();

	Ball* bestBall = nullptr;
	uint16_t maxRadius = 0;
	for(auto& ball: ballInfo->balls){
		if(ball.r > maxRadius){
			maxRadius = ball.r;
			bestBall = &ball;
		}
	}

	if(maxRadius >= StopRadius){
		Motors.setAll(0);
		return;
	}

	int currentX;
	if(bestBall == nullptr || maxRadius == 0){
		if(lastX == -1 || noBallCounter > 4){
			Motors.setAll(0);
			return;
		}else{
			noBallCounter++;
			currentX = lastX;
		}
	}else{
		currentX = bestBall->pos.x;
		noBallCounter = 0;
	}

	float amt = abs(80.0 - (float) currentX) / 80.0;
	float amtR, amtL;
	if(amt <= 0.1){
		amtL = 100;
		amtR = 100;
	}else{
		if(currentX < 80){
			amtR = 60.0f * amt + 40.0f;
			amtL = 0;
		}else{
			amtR = 0;
			amtL = 60.0f * amt + 40.0f;
		}
	}

	lastX = currentX;

	if(motorsLocked) return;

	Motors.setLeft(amtL);
	Motors.setRight(amtR);
}

void BallDriver::onBallHue(uint8_t hue){
	this->hue = hue;
	Underlights.setSolid(ColorConverter::toRGB(hue));
	S3.setHue(hue);
}

void BallDriver::onMotorsTimeout(uint8_t duration){
	Motors.stopAll();
	motorsLocked = true;
}

void BallDriver::onMotorsTimeoutClear(){
	motorsLocked = false;
}
