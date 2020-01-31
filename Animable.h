#pragma once

class Animable
{
protected:
	int nFrames;
	int curFrame = 0;
	float frameTime;
	float curframetime = 0.0f;
public:
	Animable(int nFrames, float frameTime);
	void update(float dt);
	virtual void onFrameChanged() = 0;
	void setFrame(int frame);
};

