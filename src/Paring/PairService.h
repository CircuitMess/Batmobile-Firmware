
#ifndef BATMOBILE_FIRMWARE_PAIRSERVICE_H
#define BATMOBILE_FIRMWARE_PAIRSERVICE_H

#include <Arduino.h>
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
	virtual ~PairService();
	void start();
	void stop();

    void setDoneCallback(std::function<void()> callback);

private:
	Pair::State* currentState = nullptr;
	void setState(Pair::State* state);

	void paringDone(std::unique_ptr<AsyncClient> client);
	std::function<void()> doneCallback = {};

	friend class StreamConState;
	friend class WiFiConState;
	friend class ScanState;

};
#endif //BATMOBILE_FIRMWARE_PAIRSERVICE_H
