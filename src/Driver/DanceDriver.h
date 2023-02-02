#ifndef BATMOBILE_FIRMWARE_DANCEDRIVER_H
#define BATMOBILE_FIRMWARE_DANCEDRIVER_H


#include <Loop/LoopListener.h>
#include "Driver.h"

class DanceDriver : public Driver, private ComListener, private LoopListener {
public:
    DanceDriver();
    ~DanceDriver() override;
    void onFrame(DriveInfo &driveInfo) override;

private:
    void onDance(DanceType dance) override;
    void loop(uint micros) override;
protected:
	void onStart() override;
	void onStop() override;
private:

	MotorInfo danceInfo[4][2] = {{{0,   0,   0,   0},   {0,   0,   0,   0}},
                                 {{40,  -40, 40,  -40}, {-40, 40,  -40, 40}},
                                 {{60,  60,  60,  60},  {-60, -60, -60, -60}},
                                 {{-100, 100,  -100, 100},  {100,  -100, 100,  -100}}};
    uint32_t switchDelay = 1500000;
    uint32_t count = 0;
    bool danceFlag = true;
    DanceType currentDance = DanceType::Idle;
};


#endif //BATMOBILE_FIRMWARE_DANCEDRIVER_H
