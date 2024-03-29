#include "DriveState.h"
#include "../Driver/ManualDriver.h"
#include "../Driver/DanceDriver.h"
#include "../Driver/SimpleProgDriver.h"
#include "../Driver/MarkerDriver.h"
#include "../Driver/BallDriver.h"
#include <Loop/LoopManager.h>
#include <Batmobile.h>

DriveState::DriveState(DriveMode mode){
	// If mode is idle, do nothing (setMode returns early)
	setMode(mode);
}

void DriveState::onStart(){
	Taillights.clear();
	Headlights.clear();
	Underlights.clear();

	if(currentMode != DriveMode::SimpleProgramming){
		Audio.play(SPIFFS.open("/SFX/driverStart.aac"));
	}

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
	Motors.stopAll();

	if(currentMode != DriveMode::SimpleProgramming){
		Audio.play(SPIFFS.open("/SFX/driverExit.aac"));
	}

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
			{ DriveMode::Dance,  [](){ return std::make_unique<DanceDriver>(); }},
			{ DriveMode::SimpleProgramming,  [](){ return std::make_unique<SimpleProgDriver>(); }}
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
	if(cameraError) return;

	frameTime += micros;
	if(frameTime < FrameInterval) return;
	frameTime -= FrameInterval;

	auto info = S3.getFrame();
	if(info == nullptr || info->frame.size == 0 || info->frame.data == nullptr){
		if(S3.getError() == S3Error::Camera){
			cameraError = true;
			Com.sendError(BatError::Camera);
		}
		return;
	}

	if(driver && info->mode == driver->getMode()){
		driver->onFrame(*info);
	}

	info->motors = Motors.getAll();
	feed.sendFrame(*info);
}
