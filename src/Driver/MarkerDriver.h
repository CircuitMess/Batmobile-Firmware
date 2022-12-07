#ifndef BATMOBILE_FIRMWARE_MARKERDRIVER_H
#define BATMOBILE_FIRMWARE_MARKERDRIVER_H

#include "Driver.h"
#include <Loop/LoopListener.h>


class MarkerDriver : public Driver, private LoopListener {
public:
	~MarkerDriver() override = default;

	void onFrame(DriveInfo& driveInfo) override;

private:
	void processAction(MarkerAction action);
	void loop(uint micros) override;

	MarkerAction current = MarkerAction::None;

	uint32_t continuousActionTimer = 0;
	static constexpr uint32_t burnoutDuration = 3000000; //3s
	static constexpr uint32_t do360Duration = 1750000; //1.75s

	static constexpr int8_t motorsSpeed = 100;
};


#endif //BATMOBILE_FIRMWARE_MARKERDRIVER_H
