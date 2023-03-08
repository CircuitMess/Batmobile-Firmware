#ifndef BATMOBILE_FIRMWARE_DRIVERCONTROLS_H
#define BATMOBILE_FIRMWARE_DRIVERCONTROLS_H


#include <Communication/ComListener.h>
#include "../Lightshow/Lightshow.h"

class DriverControls : private ComListener {
public:
	void start();
	void stop();
	[[nodiscard]] uint8_t getDriveSpeed() const;
	void setLightshowDisable(bool lightshowDisable);

private:
	void onHonk() override;
	void onDriveSpeed(uint8_t speed) override;
	void onBoost(bool boost) override;

	uint8_t driveSpeed = 0;
	bool boosting = false;

	bool lightshowDisable = true;
	std::unique_ptr<Lightshow> lightshow;
	LightshowType lightshowType = LightshowType::None;
	LightshowType prevType = LightshowType::None;

};


#endif //BATMOBILE_FIRMWARE_DRIVERCONTROLS_H
