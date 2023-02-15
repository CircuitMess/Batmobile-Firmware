#ifndef BATMOBILE_FIRMWARE_SOLIDLIGHTSHOW_H
#define BATMOBILE_FIRMWARE_SOLIDLIGHTSHOW_H

#include "Lightshow.h"

class SolidLightshow : public Lightshow {
protected:
	void onStart() override;
};


#endif //BATMOBILE_FIRMWARE_SOLIDLIGHTSHOW_H
