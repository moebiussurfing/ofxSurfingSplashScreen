#pragma once

#include "ofMain.h"

#include "ofxSurfingSplashScreen.h"

class ofApp : public ofBaseApp
{
public:

	void setup();
	void draw();
	void keyPressed(int key);

	ofxSurfingSplashScreen splash;

	//--

	void drawScene();

	ofColor c1 = ofColor::blue;
	ofColor c2 = ofColor::white;

	//--

	/* Return a linear value in range [0,1] every delay (in seconds). */
	float Tick(float delay = 1.0f) {
		return fmodf(ofGetElapsedTimeMillis() / 1000.0f, delay) / delay;
	}
	/* Return a linear value in range [0,1] every delay (in seconds),
	 with value equal 0 at boundaries, and 1 at delay / 2. */
	float Bounce(float delay = 1.0f) {
		return 0.5f * (1.0 + glm::sin(Tick(delay) * glm::two_pi<float>()));
	}
	/* Noise function used by the gradient scaling. */
	float Noise(const ofPoint& vertex = ofPoint(1, -1)) {
		return ofNoise(0.05f * vertex + 0.5f * ofGetElapsedTimeMillis() * 0.002f);
	}

};
