#ifndef BATMOBILE_FIRMWARE_FEEDSERVICE_H
#define BATMOBILE_FIRMWARE_FEEDSERVICE_H

#include <Loop/LoopListener.h>
#include <AsyncUDP.h>
#include <DriveInfo.h>

class FeedService {
public:
	FeedService();
	~FeedService();

	void sendFrame(const DriveInfo& camFrame);

private:
	AsyncUDP udp;
	constexpr static uint16_t port = 5000;
};

#endif //BATMOBILE_FIRMWARE_FEEDSERVICE_H
