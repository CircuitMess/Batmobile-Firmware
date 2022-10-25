#ifndef BATMOBILE_FIRMWARE_STATE_H
#define BATMOBILE_FIRMWARE_STATE_H


class State {
public:
	State();
	virtual ~State() = default;
	void start();
	void stop();
	static State* currentState;

protected:
	virtual void onStart();
	virtual void onStop();

private:
	bool started = false;
};


#endif //BATMOBILE_FIRMWARE_STATE_H
