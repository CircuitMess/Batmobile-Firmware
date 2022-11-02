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

	constexpr static size_t JpgMaxSize = 4500;
	static constexpr size_t TxBufSize = 3 * (sizeof(DriveInfo) + JpgMaxSize);
	uint8_t* txBuf;

};

#endif //BATMOBILE_FIRMWARE_FEED_H
