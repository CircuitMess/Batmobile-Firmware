#ifndef BATMOBILE_FIRMWARE_BALLDRIVER_H
#define BATMOBILE_FIRMWARE_BALLDRIVER_H


#include "Driver.h"

class BallDriver : public Driver, private ComListener {
public:
	BallDriver();
	~BallDriver() override;

	void onFrame(DriveInfo& driveInfo) override;

protected:
	void onStart() override;
	void onStop() override;

private:
	void onBallHue(uint8_t hue) override;
	uint8_t hue = 110;

	int lastX = -1;
	uint8_t noBallCounter = 0;
	static constexpr uint16_t StopRadius = 50;

};


#endif //BATMOBILE_FIRMWARE_BALLDRIVER_H
