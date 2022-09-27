
#ifndef BATMOBILE_FIRMWARE_PAIRSERVICE_H
#define BATMOBILE_FIRMWARE_PAIRSERVICE_H


namespace Pair{
    class State;
    class PairService;
    class ScanState;
    class WiFiConState;
    class SteamConState;
}
class Pair::PairService {
public:
    PairService();
    ~PairService();
    void setState(Pair::State* state);
    void doneCallback();
private:
    Pair::State* currentState;
};
#endif //BATMOBILE_FIRMWARE_PAIRSERVICE_H
