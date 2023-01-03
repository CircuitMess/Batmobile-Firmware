#ifndef BATMOBILE_FIRMWARE_SCANSTATE_H
#define BATMOBILE_FIRMWARE_SCANSTATE_H

#include "PairService.h"
#include "State.h"
#include <Loop/LoopListener.h>

class Pair::ScanState : public Pair::State, private LoopListener{
public:
    ScanState(Pair::PairService *pairService);

protected:
    void onStart() override;
    void onStop() override;

private:
    void loop(uint micros) override;

	static constexpr uint32_t pairTimeout = 180000000; //3min
	uint32_t timeoutCounter = 0;
};


#endif //BATMOBILE_FIRMWARE_SCANSTATE_H
