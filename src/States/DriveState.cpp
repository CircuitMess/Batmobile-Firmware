#include "DriveState.h"
#include "../Driver/ManualDriver.h"
#include <Batmobile.h>
#include <Loop/LoopManager.h>

DriveState::DriveState(){

}

DriveState::~DriveState(){

}

void DriveState::onStart(){
	LoopManager::addListener(this);
}

void DriveState::onStop(){
	LoopManager::removeListener(this);
}

void DriveState::setMode(DriveMode mode){
	switch(mode){
		case Manual:
			driver = std::make_unique<ManualDriver>();
			break;
		case Ball:
			break;
		case Line:
			break;
		case Marker:
			break;
	}
}

void DriveState::loop(uint micros){
	auto f = S3.getFrame();
	feed.sendFrame(*f);
	driver->onFrame(*f);
}
