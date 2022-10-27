#ifndef BATMOBILE_FIRMWARE_DRIVESTATE_H
#define BATMOBILE_FIRMWARE_DRIVESTATE_H

#include "State.h"
#include "../Driver/Driver.h"
#include <memory>
#include <DriveMode.h>
#include <Loop/LoopListener.h>

class Feed {
public:
	void sendFrame(const DriveInfo& driveInfo);
};

class DriveState : public State, private LoopListener {
public:
	DriveState();
	~DriveState();

	void setMode(DriveMode mode);

protected:
	void onStart() override;
	void onStop() override;

private:
	void loop(uint micros) override;
	Feed feed;
	std::unique_ptr<Driver> driver;
};


#endif //BATMOBILE_FIRMWARE_DRIVESTATE_H
