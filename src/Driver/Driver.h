#ifndef BATMOBILE_FIRMWARE_DRIVER_H
#define BATMOBILE_FIRMWARE_DRIVER_H

#include <DriveInfo.h>
#include "DriverControls.h"

class Driver {
public:
	virtual ~Driver();

	virtual void onFrame(DriveInfo& driveInfo) = 0;

	DriveMode getMode() const;

	void start();
	void stop();

protected:
	explicit Driver(DriveMode mode);

	DriveMode mode;

	DriverControls controls;

	bool started = false;
	virtual void onStart();
	virtual void onStop();

};


#endif //BATMOBILE_FIRMWARE_DRIVER_H
