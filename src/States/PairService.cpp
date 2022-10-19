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

void Pair::PairService::doneCallback(AsyncClient *client) {
    delete this->client;
    this->client = client;
    currentState->stop();
    delete currentState;
}

AsyncClient *Pair::PairService::getClient() {
    return client;
}
