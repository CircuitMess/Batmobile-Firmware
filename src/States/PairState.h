#ifndef BATMOBILE_FIRMWARE_PAIRSTATE_H
#define BATMOBILE_FIRMWARE_PAIRSTATE_H

#include "State.h"
#include "../Paring/PairService.h"

class PairState : public State{
public:
	PairState();
	~PairState() override;

protected:
	void onStart() override;
	void onStop() override;

private:
	Pair::PairService pair;

};


#endif //BATMOBILE_FIRMWARE_PAIRSTATE_H
