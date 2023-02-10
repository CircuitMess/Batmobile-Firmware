#include "DriveState.h"
#include "../Driver/ManualDriver.h"
#include "../Driver/DanceDriver.h"
#include "../Driver/MarkerDriver.h"
#include "../Driver/BallDriver.h"
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

	if(driver){
		driver->start();
	}
}

void DriveState::onStop(){
	if(driver){
		driver->stop();
	}

	Taillights.clear();
	Headlights.clear();
	Underlights.clear();

	Audio.play(SPIFFS.open("/SFX/driverExit.aac"));

	LoopManager::removeListener(this);
}

void DriveState::setMode(DriveMode newMode){
	if(currentMode == newMode) return;

	if(driver){
		driver->stop();
		driver.reset();
	}

	static const std::map<DriveMode, std::function<std::unique_ptr<Driver>()>> starter = {
			{ DriveMode::Idle,   [](){ return nullptr; }},
			{ DriveMode::Manual, [](){ return std::make_unique<ManualDriver>(); }},
			{ DriveMode::Ball,   [](){ return std::make_unique<BallDriver>(); }},
			{ DriveMode::Line,   [](){ return nullptr; }},
			{ DriveMode::Marker, [](){ return std::make_unique<MarkerDriver>(); }},
			{ DriveMode::QRScan, [](){ return nullptr; }},
			{ DriveMode::Dance,  [](){ return std::make_unique<DanceDriver>(); }}
	};

	if(!starter.count(newMode)){
		currentMode = DriveMode::Idle;
		return;
	}

	driver = starter.at(newMode)();

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

	if(driver && info->mode == driver->getMode()){
		driver->onFrame(*info);
	}

	info->motors = Motors.getAll();
	feed.sendFrame(*info);
}
