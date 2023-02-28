
#ifndef BATMOBILE_FIRMWARE_POLICELIGHTSHOW_H
#define BATMOBILE_FIRMWARE_POLICELIGHTSHOW_H


#include "Lightshow.h"

class PoliceLightshow : public Lightshow, private LoopListener {
protected:
	void onStart() override;
	void onStop() override;

private:
	void loop(uint micros) override;

	uint32_t counter = 0;
	bool blue = false;
	static constexpr uint32_t BlinkInterval = 250000;
};


#endif //BATMOBILE_FIRMWARE_POLICELIGHTSHOW_H
