#ifndef BATMOBILE_FIRMWARE_SIMPLEPROGDRIVER_H
#define BATMOBILE_FIRMWARE_SIMPLEPROGDRIVER_H

#include "Driver.h"
#include <DriveDirection.h>

class SimpleProgDriver : public Driver, private ComListener, private LoopListener {
public:
	SimpleProgDriver();
	~SimpleProgDriver() override;
	void onFrame(DriveInfo& driveInfo) override;

protected:
	void onStart() override;
	void onStop() override;

private:
	void onDriveDir(uint8_t dir) override;
	void onDriveSpeed(uint8_t speed) override;
	void onHeadlights(uint8_t val) override;
	void onTaillights(uint8_t val) override;
	void onUnderlights(uint8_t color) override;
	void onSoundEffect(uint8_t sample) override;
	void onMotorsTimeout(uint8_t duration) override;
	void onMotorsTimeoutClear() override;

	static constexpr uint8_t NumColors = 5;

	float speed = 0;
	DriveDirection direction = DriveDirection::None;
	void setMotors();

	void loop(uint micros) override;
	size_t motorsTimeout = 0;
	size_t timer = 0;
	bool motorsLocked = false;

	static constexpr int16_t speedStraight = 100;
	static constexpr int16_t speedTurnOuter = 100;
	static constexpr int16_t speedTurnInner = -100;
	static constexpr int16_t speedLightTurnOuter = 140;
	static constexpr int16_t speedLightTurnInner = 50;
};


#endif //BATMOBILE_FIRMWARE_SIMPLEPROGDRIVER_H
