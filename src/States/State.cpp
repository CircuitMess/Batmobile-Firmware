#include "State.h"
using namespace Pair;

State::State(PairService* pairService): pairService(pairService){}

void State::start(){
    if(started) return;
    started = true;
    onStart();
}

void State::stop(){
    if(!started) return;
    started = false;
    onStop();
}
