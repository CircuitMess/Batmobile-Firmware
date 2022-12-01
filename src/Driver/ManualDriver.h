#ifndef BATMOBILE_FIRMWARE_MANUALDRIVER_H
#define BATMOBILE_FIRMWARE_MANUALDRIVER_H

#include "Driver.h"
#include <Communication/ComListener.h>
#include <Loop/LoopListener.h>
#include <DriveDirection.h>

//TODO - register to Comms and forward controls to motors, taking boost into account

class ManualDriver : public Driver, private ComListener, private LoopListener {
public:
	ManualDriver();
	virtual ~ManualDriver();

	void onFrame(DriveInfo& driveInfo) override;

private:
	void onBoost(bool boost) override;
	void onDriveDir(uint8_t dir) override;
	void setMotors();

	void loop(uint micros) override;
	static constexpr size_t directionReceiveInterval = 1000000; //stop if no direction was received after 1s
	size_t directionTimeout = 0;

	bool boosting = false;
	uint8_t direction = 0;
	DriveDirection parsedDirection = DriveDirection::None;
	bool drifting = false;

	static constexpr int8_t speedStraight = 100;
	static constexpr int8_t speedTurnOuter = 100;
	static constexpr int8_t speedTurnInner = -20;
	static constexpr int8_t speedLightTurnOuter = 100;
	static constexpr int8_t speedLightTurnInner = 20;
	static constexpr float noBoostMultiplier = 0.4;
};


#endif //BATMOBILE_FIRMWARE_MANUALDRIVER_H
