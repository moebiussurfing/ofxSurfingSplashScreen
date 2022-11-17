#pragma once

#include "ofMain.h"

#include "ofxSurfingSplashScreen.h"

class ofApp : public ofBaseApp 
{
public:

	void setup();
	void draw();
	void keyPressed(int key);

	void drawScene();
	ofColor c = ofColor::blue;

	ofxSurfingSplashScreen splash;
};
