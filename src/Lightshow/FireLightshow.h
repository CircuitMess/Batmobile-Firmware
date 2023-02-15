#ifndef BATMOBILE_FIRMWARE_FIRELIGHTSHOW_H
#define BATMOBILE_FIRMWARE_FIRELIGHTSHOW_H

#include "Lightshow.h"
#include <Loop/LoopListener.h>

class FireLightshow : public Lightshow, private LoopListener {
protected:
	void onStart() override;
	void onStop() override;
private:
	void loop(uint micros) override;

	uint32_t fireRandomDuration = 0;
	uint32_t fireTimer = 0;
};


#endif //BATMOBILE_FIRMWARE_FIRELIGHTSHOW_H
