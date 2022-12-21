#ifndef BATMOBILE_FIRMWARE_PAIRSTATE_H
#define BATMOBILE_FIRMWARE_PAIRSTATE_H

#include <Loop/LoopListener.h>
#include "State.h"
#include "../Paring/PairService.h"

class PairState : public State, private LoopListener {
public:
	PairState();
	~PairState();

protected:
	void onStart() override;
	void onStop() override;

private:
	Pair::PairService pair;

	void loop(uint micros) override;

	static constexpr uint32_t pairTimeout = 180000000; //3min
	uint32_t timeoutCounter = 0;
};


#endif //BATMOBILE_FIRMWARE_PAIRSTATE_H
