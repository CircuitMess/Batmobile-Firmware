#ifndef BATMOBILE_FIRMWARE_IDLESTATE_H
#define BATMOBILE_FIRMWARE_IDLESTATE_H

#include "State.h"
#include <Communication/ComListener.h>

class IdleState : public State, private ComListener{
public:
	IdleState();
	~IdleState();

protected:
	void onStart() override;
	void onStop() override;

private:
	void onVolume(uint8_t volume) override;
protected:
	void onSettingsSound() override;

};


#endif //BATMOBILE_FIRMWARE_IDLESTATE_H
