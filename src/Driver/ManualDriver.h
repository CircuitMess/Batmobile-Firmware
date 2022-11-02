#ifndef BATMOBILE_FIRMWARE_MANUALDRIVER_H
#define BATMOBILE_FIRMWARE_MANUALDRIVER_H

#include "Driver.h"
#include <Communication/ComListener.h>

//TODO - register to Comms and forward controls to motors, taking boost into account

class ManualDriver : public Driver , private ComListener{
public:
	ManualDriver();
	void onFrame(DriveInfo& driveInfo) override;

private:
	void onBoost(bool boost) override;
	void onDriveDir(DriveDirection dir) override;
	void setMotors();

	bool boosting = false;
	DriveDirection direction{};

	static constexpr uint8_t speedStraight = 100;
	static constexpr uint8_t speedTurnOuter = 100;
	static constexpr uint8_t speedTurnInner = 20;
	static constexpr float noBoostMultiplier = 0.7;
};


#endif //BATMOBILE_FIRMWARE_MANUALDRIVER_H
