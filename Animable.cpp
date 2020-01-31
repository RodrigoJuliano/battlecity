#include "Animable.h"

Animable::Animable(int nFrames, float frameTime)
	:
	nFrames(nFrames),
	frameTime(frameTime)
{
}

void Animable::update(float dt) {
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

void Animable::setFrame(int frame) {
	curFrame = frame;
	onFrameChanged();
}
