#ifndef BATMOBILE_FIRMWARE_BALLDRIVER_H
#define BATMOBILE_FIRMWARE_BALLDRIVER_H


#include <Communication/ComListener.h>
#include <Loop/LoopListener.h>
#include "Driver.h"

class BallDriver : public Driver, private ComListener, private LoopListener{
public:
    BallDriver();
    ~BallDriver() override;

    void onFrame(DriveInfo &driveInfo) override;

private:

    void loop(uint micros) override;

    void onBallHue(uint8_t hue) override;

    void onDriveSpeed(uint8_t speed) override;

protected:
    void onStart() override;

    void onStop() override;
};


#endif //BATMOBILE_FIRMWARE_BALLDRIVER_H
