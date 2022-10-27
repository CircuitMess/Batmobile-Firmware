#ifndef BATMOBILE_FIRMWARE_DRIVER_H
#define BATMOBILE_FIRMWARE_DRIVER_H

class Driver {
public:
	virtual void onFrame(DriveInfo& driveInfo) = 0;
};


#endif //BATMOBILE_FIRMWARE_DRIVER_H
