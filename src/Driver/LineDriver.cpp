#include <cstdlib>
#include <Batmobile.h>
#include "LineDriver.h"

LineDriver::LineDriver() : Driver(DriveMode::Line){

}

LineDriver::~LineDriver(){
	stop();
}

void LineDriver::onStart(){
	LoopManager::addListener(this);
}

void LineDriver::onStop(){
	LoopManager::removeListener(this);
}

void LineDriver::onFrame(DriveInfo& driveInfo){ }

void LineDriver::loop(uint micros){
	data = S3.getProximity();
	printf("Back / Left / Right | %4d / %4d / %4d\n", data.mid, data.left, data.right);

	for(int i = 0; i < 3; i++){
		if(i == 0) continue;
		if(data.raw[i] < 250){
			moveStop();
			resetLimits();
			lineSide = None;
			return;
		}
	}

	setLimits();

	auto lastSide = lineSide;
	if(lineLeft() && lineRight()){
		lineSide = Straight;
	}else if(lineLeft()){
		lineSide = Left;
	}else if(lineRight()){
		lineSide = Right;
	}else if(lastSide == Right && movingDir == Right){
		lineSide = Straight;
	}else if(lastSide == Left && movingDir == Left){
		lineSide = Straight;
	}else{
		lineSide = None;
	}

	if(lineSide == None && lastSide != None){
		lastSeenSide = lastSide;
	}else if(lastSide != None){
		lastSeenSide = lineSide;
	}

	if(lineSide == None && lastSeenSide == Right){
		moveRight();
	}else if(lineSide == None && lastSeenSide == Left){
		moveLeft();
	}else if(lineSide != None){
		moveStraight();
	}
}

void LineDriver::setLimits(){
	for(int i = 0; i < 3; i++){
		limits.min.raw[i] = std::min(limits.min.raw[i], data.raw[i]);
		limits.max.raw[i] = std::max(limits.max.raw[i], data.raw[i]);
	}
}

void LineDriver::resetLimits(){
	for(int i = 0; i < 3; i++){
		limits.min.raw[i] = limits.max.raw[i] = 350;
	}
}

bool LineDriver::lineMid(){
	return line(data.mid, limits.min.mid, limits.max.mid);
}

bool LineDriver::lineLeft(){
	return line(data.left, limits.min.left, limits.max.left);
}

bool LineDriver::lineRight(){
	return line(data.right, limits.min.right, limits.max.right);
}

bool LineDriver::line(uint16_t val, uint16_t min, uint16_t max){
	if(max - min < 100) return false;
	auto mid = (max + min) / 2;
	return val < mid;
}

void LineDriver::moveStraight(){
	movingDir = Straight;
	Motors.setAll(60);
}

void LineDriver::moveLeft(){
	movingDir = Left;
	Motors.setLeft(-100);
	Motors.setRight(100);
}

void LineDriver::moveRight(){
	movingDir = Right;
	Motors.setLeft(100);
	Motors.setRight(-100);
}

void LineDriver::moveStop(){
	movingDir = None;
	Motors.stopAll();
}
