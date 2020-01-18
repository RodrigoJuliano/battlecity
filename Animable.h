#pragma once

class Animable
{
protected:
	const int nFrames;
	int curFrame = 0;
	const float frameTime;
	float curframetime = 0.0f;
public:
	inline Animable(int nFrames, float frameTime)
	:
		nFrames(nFrames),
		frameTime(frameTime)
	{
	}
	inline void update(float dt) {
		if (nFrames > 1) {
			if (curframetime > frameTime) {
				curframetime = 0.0f;
				// change the frame
				curFrame++;
				if (curFrame > nFrames - 1)
					curFrame = 0;
				onFrameChanged();
			}
			else {
				curframetime += dt;
			}
		}
	}
	virtual void onFrameChanged() = 0;
};

