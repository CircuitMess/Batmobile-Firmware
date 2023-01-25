#ifndef BATMOBILE_FIRMWARE_BALLDRIVER_H
#define BATMOBILE_FIRMWARE_BALLDRIVER_H


#include "Driver.h"

class BallDriver : public Driver, private ComListener {
public:
	BallDriver();
	virtual ~BallDriver();

	void onFrame(DriveInfo& driveInfo) override;

private:
	void onBallHue(uint8_t hue) override;

};


#endif //BATMOBILE_FIRMWARE_BALLDRIVER_H
