#pragma once

#include "ofMain.h"

#include "ofxSurfingSplashScreen.h"

class ofApp : public ofBaseApp 
{
public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key);

	ofxSurfingSplashScreen splash;

	void drawScene();
	ofColor c = ofColor::blue;
};
