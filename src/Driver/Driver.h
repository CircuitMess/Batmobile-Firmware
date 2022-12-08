#ifndef BATMOBILE_FIRMWARE_DRIVER_H
#define BATMOBILE_FIRMWARE_DRIVER_H

#include <DriveInfo.h>

class Driver {
public:
	virtual ~Driver(){ }

	virtual void onFrame(DriveInfo& driveInfo) = 0;
};


#endif //BATMOBILE_FIRMWARE_DRIVER_H
