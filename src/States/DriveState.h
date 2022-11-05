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
};


#endif //BATMOBILE_FIRMWARE_DRIVESTATE_H
