#ifndef BATMOBILE_FIRMWARE_IDLESTATE_H
#define BATMOBILE_FIRMWARE_IDLESTATE_H

#include "State.h"
#include <Communication/ComListener.h>
#include <Loop/LoopListener.h>

class IdleState : public State, private ComListener, private LoopListener{
public:
	IdleState();
	~IdleState() override;

protected:
	void onStart() override;
	void onStop() override;

	void onSettingsSound() override;

private:
	void loop(uint micros) override;
};


#endif //BATMOBILE_FIRMWARE_IDLESTATE_H
