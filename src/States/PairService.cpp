#include "PairService.h"
#include "State.h"
#include "ScanState.h"
#include "WiFiConState.h"

Pair::PairService::PairService() {
    currentState = new Pair::ScanState(this);
    currentState->start();
}

Pair::PairService::~PairService(){

}

void Pair::PairService::setState(Pair::State* state){
    currentState->stop();
    delete currentState;
    currentState = state;
    currentState->start();
}
