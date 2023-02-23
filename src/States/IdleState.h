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

private:
	void onSettingsSound() override;
	void onSoundEffect(uint8_t sample) override;

	void loop(uint micros) override;
	uint32_t counter = 0;
	uint32_t threshold = 10000000;
};


#endif //BATMOBILE_FIRMWARE_IDLESTATE_H
