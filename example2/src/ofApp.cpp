#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetCircleResolution(100);

	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	ofSetWindowShape(1800, 950);
	ofSetWindowPosition(50, 50);

	//--

	splash.setup("assets/images/Paletto_Banner.png");
	//splash.setup("assets/images/Aldous.png");

	//--
	
	//splash.setDuration(1000); 
	
	// (false) no floating means window is not resized nor borderless mode!
	//splash.setModeFloating(false); 
}

//--------------------------------------------------------------
void ofApp::update() 
{
	string s = ofToString(ofGetFrameRate(), 0) + " FPS " + splash.getDebugInfo();
	ofSetWindowTitle(s);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofClear(c);

	if (splash.draw()) return;
	// Skipped during splashing

	//--

	drawScene();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	const int d = 30;
	float sz = max(ofGetWidth(), ofGetHeight());
	float r1 = ofMap(ofGetFrameNum() % d, 0, d, 0, 1, true) * sz;
	float r2 = ofMap(ofGetFrameNum() % d, 0, d, 0, 1, true) * sz * 0.2;
	float r3 = ofMap(ofGetFrameNum() % d, 0, d, 0, 1, true) * sz * 0.025;
	ofPushStyle();
	ofFill();
	ofSetColor(255, 255);
	ofDrawCircle(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, 0, r1);
	ofSetColor(c, 255);
	ofDrawCircle(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, 0, r2);
	ofSetColor(255, 255);
	ofDrawCircle(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, 0, r3);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	// Splash!
	if (key == ' ') splash.start();

	// Debug
#if defined(TARGET_WIN32)
	if (key == OF_KEY_F1)
	{
		// Make app always on top
		HWND W = GetActiveWindow();
		SetWindowPos(W, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
	}

	if (key == OF_KEY_F2)
	{
		// Disable make app always on top
		HWND W = GetActiveWindow();
		SetWindowPos(W, HWND_NOTOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
	}

	if (key == OF_KEY_F3)
	{
		splash.setToggleBorderless();
	}

	if (key == OF_KEY_F4)
	{
		splash.setToggleModeFloating();
	}
#endif
}
