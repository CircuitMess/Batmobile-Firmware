#include "SimplePlaybackSystem.h"
#include <SPIFFS.h>
#include <Batmobile.h>

SimplePlaybackSystem::SimplePlaybackSystem() : task("Playback", [](Task* task){
	auto system = static_cast<SimplePlaybackSystem*>(task->arg);
	system->player();
}, 4096, this), output(PIN_SPKR), playQueue(4, sizeof(PlayJob)){

	output.setSource(source);
}

SimplePlaybackSystem::~SimplePlaybackSystem(){
	stop();
}

void SimplePlaybackSystem::begin(){
	if(task.running) return;

	task.start(1, 0);
}

void SimplePlaybackSystem::stop(){
	task.stop(true);
	delete source;
	delete dataSource;
}

void SimplePlaybackSystem::play(File file){
	auto jobDataSource = new FileDataSource(file);
	auto jobSource = new SourceAAC(*jobDataSource);

	PlayJob job = { jobDataSource, jobSource };
	playQueue.send(&job);

	running = true;
}

void SimplePlaybackSystem::player(){
	while(task.running){
		while(playQueue.count()){
			PlayJob next{};
			if(!playQueue.receive(&next)) continue;

			if(source){
				source->close();
			}

			delete source;
			delete dataSource;

			dataSource = next.dataSource;
			source = next.source;

			output.setSource(source);
			output.start();
		}

		if(output.isRunning()){
			output.loop(0);
			delay(10);
			if(!output.isRunning()){
				running = false;
			}
		}
	}
}

void SimplePlaybackSystem::setVolume(uint8_t volume){
	output.setGain((float) volume / 255.0f);
}

bool SimplePlaybackSystem::isPlaying() const{
	return running;
}
