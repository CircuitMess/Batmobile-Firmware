#ifndef BATMOBILE_FIRMWARE_DRIVESTATE_H
#define BATMOBILE_FIRMWARE_DRIVESTATE_H

#include "State.h"

class DriveState : public State{
public:
	DriveState();
	~DriveState();

protected:
	void onStart() override;
	void onStop() override;

};


#endif //BATMOBILE_FIRMWARE_DRIVESTATE_H
