#ifndef BATMOBILE_FIRMWARE_FEED_H
#define BATMOBILE_FIRMWARE_FEED_H

#include <Loop/LoopListener.h>
#include <AsyncUDP.h>
#include <DriveInfo.h>

class Feed {
public:
	Feed();
	~Feed();

	void sendFrame(const DriveInfo& frame);

private:
	AsyncUDP udp;
	constexpr static uint16_t port = 5000;

	static constexpr size_t BufSize = 6000;
	uint8_t* txBuf;

};

#endif //BATMOBILE_FIRMWARE_FEED_H
