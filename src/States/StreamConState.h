#ifndef BATMOBILE_FIRMWARE_STREAMCONSTATE_H
#define BATMOBILE_FIRMWARE_STREAMCONSTATE_H

#include <Loop/LoopListener.h>
#include "PairService.h"
#include "State.h"

class Pair::StreamConState : public Pair::State, private LoopListener{
public:
    StreamConState(Pair::PairService *pairService);
    ~StreamConState();
    void loop(uint micros) override;
protected:
    void onStart() override;
    void onStop() override;
private:
    uint8_t connectTries = 0;
    AsyncClient *client;
    uint32_t time = 0;
};


#endif //BATMOBILE_FIRMWARE_STREAMCONSTATE_H
