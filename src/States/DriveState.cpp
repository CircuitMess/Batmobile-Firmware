#include "DriveState.h"
#include "../Driver/ManualDriver.h"
#include "../Driver/LineDriver.h"
#include <Loop/LoopManager.h>
#include <Batmobile.h>

DriveState::DriveState(DriveMode mode){
	// If mode is idle, do nothing (setMode returns early)
	setMode(mode);
}

void DriveState::onStart(){
	Taillights.setSolid(255);
	Headlights.setSolid(255);
	Underlights.clear();

	Audio.play(SPIFFS.open("/SFX/driverStart.aac"));

	LoopManager::addListener(this);
	frameTime = 0;
}

void DriveState::onStop(){
	Taillights.clear();
	Headlights.clear();
	Underlights.clear();

	Audio.play(SPIFFS.open("/SFX/driverExit.aac"));

	LoopManager::removeListener(this);
}

void DriveState::setMode(DriveMode newMode){
	if(currentMode == newMode) return;

	driver.reset();

	static const std::function<std::unique_ptr<Driver>()> starter[5] = {
			[](){ return nullptr; },
			[](){ return std::make_unique<ManualDriver>(); },
			[](){ return nullptr; },
			[](){ return std::make_unique<LineDriver>(); },
			[](){ return nullptr; },
	};

	driver = starter[(int) newMode]();
	if(driver == nullptr){
		currentMode = DriveMode::Idle;
		return;
	}

	currentMode = newMode;
}

void DriveState::loop(uint micros){
	frameTime += micros;
	if(frameTime < FrameInterval) return;
	frameTime -= FrameInterval;

	auto info = S3.getFrame();
	if(info == nullptr) return;

	if(info->mode != currentMode) return;

	driver->onFrame(*info);
	feed.sendFrame(*info);
}
