#ifndef BATMOBILE_FIRMWARE_MANUALDRIVER_H
#define BATMOBILE_FIRMWARE_MANUALDRIVER_H

#include "Driver.h"

class ManualDriver : public Driver {
public:
	void onFrame(DriveInfo& driveInfo) override;
};


#endif //BATMOBILE_FIRMWARE_MANUALDRIVER_H
