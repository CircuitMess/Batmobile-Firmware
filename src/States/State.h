#ifndef BATMOBILE_FIRMWARE_STATE_H
#define BATMOBILE_FIRMWARE_STATE_H

class State {
public:
	virtual ~State() = default;

	void start();
	void stop();
	bool isStarted() const;

protected:
	virtual void onStart(){};
	virtual void onStop(){};

private:
	bool started = false;

};

#endif //BATMOBILE_FIRMWARE_STATE_H