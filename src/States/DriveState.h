#ifndef BATMOBILE_FIRMWARE_DRIVESTATE_H
#define BATMOBILE_FIRMWARE_DRIVESTATE_H

#include "State.h"
#include "../Driver/Driver.h"
#include <memory>
#include <Loop/LoopListener.h>
#include "../Feed.h"

class DriveState : public State, private LoopListener {
public:
	DriveState(DriveMode mode);
	void setMode(DriveMode newMode);

protected:
	void onStart() override;
	void onStop() override;

private:
	void loop(uint micros) override;

	DriveMode currentMode = DriveMode::Idle;

	Feed feed;
	std::unique_ptr<Driver> driver;

	uint32_t frameTime = 0;
	static constexpr uint32_t FrameInterval = 50000;

};


#endif //BATMOBILE_FIRMWARE_DRIVESTATE_H
