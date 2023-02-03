#ifndef BATMOBILE_FIRMWARE_LINEDRIVER_H
#define BATMOBILE_FIRMWARE_LINEDRIVER_H


#include <Loop/LoopListener.h>
#include <cstdlib>
#include "Driver.h"

class LineDriver : public Driver, private LoopListener {
public:
	LineDriver();
	~LineDriver() override;

	void onFrame(DriveInfo& driveInfo) override;

protected:
	void onStart() override;
	void onStop() override;

private:
	void loop(uint micros) override;

	struct {
		ProximityData min;
		ProximityData max;
	} limits;
	void setLimits();
	void resetLimits();

	enum Side { None, Left, Right, Straight };
	ProximityData data;
	Side lineSide = None;
	Side lastSeenSide = None;

	Side movingDir = None;
	void moveStraight();
	void moveLeft();
	void moveRight();
	void moveStop();

	bool lineMid();
	bool lineLeft();
	bool lineRight();
	bool line(uint16_t val, uint16_t min, uint16_t max);

};


#endif //BATMOBILE_FIRMWARE_LINEDRIVER_H
