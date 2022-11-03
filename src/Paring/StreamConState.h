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

};


#endif //BATMOBILE_FIRMWARE_STREAMCONSTATE_H
