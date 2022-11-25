#ifndef BATMOBILE_FIRMWARE_DRIVER_H
#define BATMOBILE_FIRMWARE_DRIVER_H

#include <DriveInfo.h>

class Driver {
public:
    virtual ~Driver() = default;

    void start();
    void stop();

	virtual void onFrame(DriveInfo& driveInfo) = 0;

protected:
    virtual void onStart(){};
    virtual void onStop(){};
};


#endif //BATMOBILE_FIRMWARE_DRIVER_H
