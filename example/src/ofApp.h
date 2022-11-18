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
};
