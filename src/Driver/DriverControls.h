#ifndef BATMOBILE_FIRMWARE_DRIVERCONTROLS_H
#define BATMOBILE_FIRMWARE_DRIVERCONTROLS_H


#include <Communication/ComListener.h>

class DriverControls : private ComListener {
public:
	void start();
	void stop();

	[[nodiscard]] bool getHeadlightsToggle() const;
	[[nodiscard]] uint8_t getDriveSpeed() const;

private:
	void onHonk() override;
	void onDriveSpeed(uint8_t speed) override;

	bool headlightsToggle = true;
    uint8_t lightsSwitch = 0;
	uint8_t driveSpeed = 0;
};


#endif //BATMOBILE_FIRMWARE_DRIVERCONTROLS_H
