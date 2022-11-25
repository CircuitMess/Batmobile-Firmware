#include "DriveState.h"
#include "../Driver/ManualDriver.h"
#include <Wheelson.h>
#include <Loop/LoopManager.h>

DriveState::DriveState(DriveMode mode){
	// If mode is idle, do nothing (setMode returns early)
	setMode(mode);
}

void DriveState::onStart(){
	LoopManager::addListener(this);
}

void DriveState::onStop(){
	LoopManager::removeListener(this);
}

void DriveState::setMode(DriveMode newMode){
	if(currentMode == newMode) return;

	driver.reset();

	static const std::function<std::unique_ptr<Driver>()> starter[5] = {
			[](){ return nullptr; },
			[](){ return std::make_unique<ManualDriver>(); },
			[](){ return std::make_unique<BallDriver>(); },
			[](){ return nullptr; },
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
	cam.loadFrame();
	auto frame = cam.getFrame();

	DriveInfo info;
	info.mode = DriveMode::Manual;
	info.frame.data = frame->buf;
	info.frame.size = frame->len;

	 driver->onFrame(info);
	feed.sendFrame(info);

	info.frame.data = nullptr;

	cam.releaseFrame();

	// TODO: uncomment once hardware is ready and S3 interface is implemented
	/*auto frame = S3.getFrame();
	if(frame->mode != currentMode) return;

	driver->onFrame(*frame);
	feed.sendFrame(*frame);*/
}
