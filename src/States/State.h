#ifndef BATMOBILE_FIRMWARE_STATE_H
#define BATMOBILE_FIRMWARE_STATE_H

#include "PairService.h"

class Pair::State {
public:
    State(Pair::PairService* pairService);
    virtual ~State() = default;

    void start();
    void stop();

protected:
    virtual void onStart() = 0;
    virtual void onStop() = 0;
    Pair::PairService* pairService;

private:
    bool started = false;
};

#endif //BATMOBILE_FIRMWARE_STATE_H
