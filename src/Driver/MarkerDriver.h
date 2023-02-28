#ifndef BATMOBILE_FIRMWARE_MARKERDRIVER_H
#define BATMOBILE_FIRMWARE_MARKERDRIVER_H

#include "Driver.h"
#include <Loop/LoopListener.h>


class MarkerDriver : public Driver, private LoopListener, private ComListener {
public:
	MarkerDriver();
	~MarkerDriver() override;

	void onFrame(DriveInfo& driveInfo) override;

protected:
	void onStart() override;
	void onStop() override;

private:
	void processAction(MarkerAction action);
	void loop(uint micros) override;

	void onMotorsTimeoutClear() override;
	void onMotorsTimeout(uint8_t duration) override;

	bool motorsLocked = false;

	MarkerAction current = MarkerAction::None;

	uint32_t continuousActionTimer = 0;
	static constexpr uint32_t burnoutDuration = 6000000; // [us]
	static constexpr uint32_t do360Duration = 8000000; // [us]
	static constexpr uint32_t explosionDuration = 6000000; // [us]
	static constexpr uint32_t batsDuration = 3000000; // [us]

	static bool continuousAction(MarkerAction action);

	static constexpr int8_t motorsSpeed = 100;

	int color = 0;
	static const glm::vec<3, uint16_t> Colors[8];

	static constexpr uint32_t ColorDuration = 1000;
	uint32_t colorTime = 0;

	bool breathing = false;

	static constexpr std::array<const char*, 13> audioFiles = { nullptr,
																nullptr,
																nullptr,
																"/Voice/360.aac",
																"/Voice/burnout.aac",
																"/Voice/headlightOn.aac",
																"/Voice/headlightOff.aac",
																"/SFX/honk0.aac",
																"/SFX/explosion.aac",
																"/Voice/underlightOff.aac",
																"/Voice/underlightBreathe.aac",
																"/Voice/underlightSolid.aac",
																"/SFX/bats.aac" };
	MarkerAction lastPlayed = MarkerAction::None;
	bool delayedVoiceDone = false;
};


#endif //BATMOBILE_FIRMWARE_MARKERDRIVER_H
