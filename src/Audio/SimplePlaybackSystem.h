#ifndef BATMOBILE_FIRMWARE_SIMPLEPLAYBACKSYSTEM_H
#define BATMOBILE_FIRMWARE_SIMPLEPLAYBACKSYSTEM_H

#include <Audio/Mixer.h>
#include <Audio/OutputDAC.h>
#include <Util/Task.h>
#include <Sync/Mutex.h>
#include <Sync/Queue.h>
#include <Audio/SourceAAC.h>
#include <Data/FileDataSource.h>

class SimplePlaybackSystem {
public:
	SimplePlaybackSystem();
	virtual ~SimplePlaybackSystem();

	void begin();

	void play(File file);
	void stop();

	void setVolume(uint8_t volume);

	[[nodiscard]] bool isPlaying() const;

private:
	OutputDAC output;

	FileDataSource* dataSource = nullptr;
	SourceAAC* source = nullptr;

	Task task;

	struct PlayJob {
		FileDataSource* dataSource;
		SourceAAC* source;
	};
	Queue playQueue;

	void player();

	bool running = false;

};

#endif //BATMOBILE_FIRMWARE_SIMPLEPLAYBACKSYSTEM_H
