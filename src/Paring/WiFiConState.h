
#ifndef BATMOBILE_FIRMWARE_WIFICONSTATE_H
#define BATMOBILE_FIRMWARE_WIFICONSTATE_H

#include <Loop/LoopListener.h>
#include "PairService.h"
#include "State.h"

class Pair::WiFiConState : public Pair::State, private LoopListener {
public:
    WiFiConState(Pair::PairService* pairService, uint16_t id);

protected:
    void onStart() override;
    void onStop() override;

private:
    void startConnection();

    char ssid[14];
    char password[10];

    uint32_t retryCounter = 0;
    uint8_t retryCount = 0;

    static constexpr uint32_t RetryInterval = 3500000;
	static constexpr uint8_t RetryTries = 3;

	void loop(uint micros) override;

};


#endif //BATMOBILE_FIRMWARE_WIFICONSTATE_H
