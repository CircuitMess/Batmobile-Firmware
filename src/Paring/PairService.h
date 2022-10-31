
#ifndef BATMOBILE_FIRMWARE_PAIRSERVICE_H
#define BATMOBILE_FIRMWARE_PAIRSERVICE_H

#include <freertos/FreeRTOS.h> //if not included an error pops up
#include <AsyncTCP.h>
#include <memory>
#include <functional>

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
    void setDoneCallback(std::function<void()> callback);

private:
	void paringDone(std::unique_ptr<AsyncClient> client);
	Pair::State* currentState;
	std::function<void()> callback = {};

	friend class StreamConState;

};
#endif //BATMOBILE_FIRMWARE_PAIRSERVICE_H
