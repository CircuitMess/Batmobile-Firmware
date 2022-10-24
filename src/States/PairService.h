
#ifndef BATMOBILE_FIRMWARE_PAIRSERVICE_H
#define BATMOBILE_FIRMWARE_PAIRSERVICE_H

#include <freertos/FreeRTOS.h> //if not included an error pops up
#include <AsyncTCP.h>

namespace Pair{
    class State;
    class PairService;
    class ScanState;
    class WiFiConState;
    class StreamConState;
}
class Pair::PairService {
public:
    PairService();
    ~PairService();
    void setState(Pair::State* state);
    void doneCallback(AsyncClient *client);
    AsyncClient* getClient();
private:
    Pair::State* currentState;
    AsyncClient* client;
    char dummyData[10];
};
#endif //BATMOBILE_FIRMWARE_PAIRSERVICE_H
