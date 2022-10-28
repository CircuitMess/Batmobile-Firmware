#ifndef BATMOBILE_FIRMWARE_PAIRSTATE_H
#define BATMOBILE_FIRMWARE_PAIRSTATE_H

#include "State.h"

class PairState : public State {
public:
	PairState();
	~PairState();

protected:
	void onStart() override;
	void onStop() override;

};


#endif //BATMOBILE_FIRMWARE_PAIRSTATE_H
