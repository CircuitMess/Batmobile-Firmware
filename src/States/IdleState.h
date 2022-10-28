#ifndef BATMOBILE_FIRMWARE_IDLESTATE_H
#define BATMOBILE_FIRMWARE_IDLESTATE_H

#include "State.h"

class IdleState : public State{
public:
	IdleState();
	~IdleState();

protected:
	void onStart() override;
	void onStop() override;

};


#endif //BATMOBILE_FIRMWARE_IDLESTATE_H
