#ifndef BATMOBILE_FIRMWARE_DONESTATE_H
#define BATMOBILE_FIRMWARE_DONESTATE_H

#include "PairService.h"
#include "State.h"

class Pair::DoneState : public Pair::State{
public:
    DoneState(Pair::PairService *pairService);
    ~DoneState();
protected:
    void onStart() override;
    void onStop() override;
private:

};


#endif //BATMOBILE_FIRMWARE_DONESTATE_H
