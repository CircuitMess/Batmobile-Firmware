
#ifndef BATMOBILE_FIRMWARE_WIFICONSTATE_H
#define BATMOBILE_FIRMWARE_WIFICONSTATE_H

#include <Loop/LoopListener.h>
#include "PairService.h"
#include "State.h"

class Pair::WiFiConState : public Pair::State, private LoopListener {
public:
    WiFiConState(Pair::PairService *pairService, uint16_t id);
    ~WiFiConState();
    void loop(uint micros) override;
protected:
    void onStart() override;
    void onStop() override;
private:
    std::string batmobile = "Batmobile";
    std::string ssid = "Batmobile ";
    char password[9];
    uint16_t id;
};


#endif //BATMOBILE_FIRMWARE_WIFICONSTATE_H
