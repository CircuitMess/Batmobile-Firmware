#ifndef BATMOBILE_FIRMWARE_MANUALDRIVER_H
#define BATMOBILE_FIRMWARE_MANUALDRIVER_H

#include "Driver.h"
#include <Communication/ComListener.h>

//TODO - register to Comms and forward controls to motors, taking boost into account

class ManualDriver : public Driver , private ComListener{
public:
	ManualDriver();
	virtual ~ManualDriver();

	void onFrame(DriveInfo& driveInfo) override;

private:
	void onBoost(bool boost) override;
	void onDriveDir(uint8_t dir) override;
	void setMotors();

	bool boosting = false;
	uint8_t direction = 0;

	static constexpr int8_t speedStraight = 100;
	static constexpr int8_t speedTurnOuter = 100;
	static constexpr int8_t speedTurnInner = -20;
	static constexpr int8_t speedLightTurnOuter = 100;
	static constexpr int8_t speedLightTurnInner = 20;
	static constexpr float noBoostMultiplier = 0.4;
};


#endif //BATMOBILE_FIRMWARE_MANUALDRIVER_H
