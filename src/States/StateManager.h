#ifndef BATMOBILE_FIRMWARE_STATEMANAGER_H
#define BATMOBILE_FIRMWARE_STATEMANAGER_H

#include <Arduino.h>
#include <DisconnectListener.h>
#include <Communication/ComListener.h>
#include <cstdint>
#include "State.h"

class StateManager : private DisconnectListener, private ComListener {
public:
	StateManager();
	virtual ~StateManager();

	void begin();

private:
	void onConnected() override;
	void onDisconnected() override;

	void onDriveMode(DriveMode mode) override;
	void onShutdown() override;

	void onVolume(uint8_t volume) override;
	void onDisconnectRequest() override;

	std::unique_ptr<State> currentState;
	DriveMode currentMode;

};


#endif //BATMOBILE_FIRMWARE_STATEMANAGER_H
