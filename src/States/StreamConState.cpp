#include "StreamConState.h"



Pair::StreamConState::StreamConState(Pair::PairService *pairService) : State(pairService) {

}

Pair::StreamConState::~StreamConState() {

}

void Pair::StreamConState::loop(uint micros) {

}

void Pair::StreamConState::onStart() {
    LoopManager::addListener(this);
}

void Pair::StreamConState::onStop() {
    LoopManager::removeListener(this);
}
