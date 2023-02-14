#ifndef BATMOBILE_FIRMWARE_STREAMCONSTATE_H
#define BATMOBILE_FIRMWARE_STREAMCONSTATE_H

#include <Loop/LoopListener.h>
#include "PairService.h"
#include "State.h"

class Pair::StreamConState : public Pair::State, private LoopListener{
public:
    StreamConState(Pair::PairService* pairService);

protected:
    void onStart() override;
    void onStop() override;

private:
	std::unique_ptr<AsyncClient> client;

	void loop(uint micros) override;
	static constexpr uint32_t ConTimeout = 3500000;
	uint32_t timeoutCounter = 0;
};


#endif //BATMOBILE_FIRMWARE_STREAMCONSTATE_H
