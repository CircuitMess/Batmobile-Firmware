#ifndef BATMOBILE_FIRMWARE_DRIVER_H
#define BATMOBILE_FIRMWARE_DRIVER_H

#include <DriveInfo.h>
#include "DriverControls.h"

class Driver {
public:
	Driver(){
		controls.start();
	}

	virtual ~Driver(){
		controls.stop();
	}

	virtual void onFrame(DriveInfo& driveInfo) = 0;

protected:
	DriverControls controls;
};


#endif //BATMOBILE_FIRMWARE_DRIVER_H
