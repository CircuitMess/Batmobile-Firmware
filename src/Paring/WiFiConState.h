
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
    void startConnection();
    char ssid[14];
    char password[10];
    uint32_t timeCounter = 0;
    uint8_t connectionTries = 0;
    static constexpr uint32_t checkInterval = 1000000;
	const uint8_t maxChecks = 5;
};


#endif //BATMOBILE_FIRMWARE_WIFICONSTATE_H