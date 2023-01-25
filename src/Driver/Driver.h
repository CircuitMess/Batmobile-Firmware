#ifndef BATMOBILE_FIRMWARE_DRIVER_H
#define BATMOBILE_FIRMWARE_DRIVER_H

#include <DriveInfo.h>
#include "DriverControls.h"

class Driver {
public:
	virtual ~Driver(){
		controls.stop();
	}

	virtual void onFrame(DriveInfo& driveInfo) = 0;

	DriveMode getMode() const{
		return mode;
	}

protected:
	Driver(DriveMode mode) : mode(mode){
		controls.start();
	}

	DriverControls controls;

	DriveMode mode;

};


#endif //BATMOBILE_FIRMWARE_DRIVER_H
