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

	//splash.setDuration(1000); 

	// No floating mode (false) 
	// means window is not resized nor border less control!
	splash.setModeFloating(false);

	//--

	splash.setup("assets/images/Paletto_Banner.png");
	//splash.setup("assets/images/Aldous.png");
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
	int f = ofGetFrameNum() % d;
	float sz = max(ofGetWidth(), ofGetHeight());
	float r1 = ofMap(f, 0, d, 0, 1, true) * sz;
	float r2 = ofMap(f, 0, d, 0, 1, true) * sz * 0.2;
	float r3 = ofMap(f, 0, d, 0, 1, true) * sz * 0.025;
	float x = ofGetWidth() * 0.5f;
	float y = ofGetHeight() * 0.5f;

	ofPushStyle();
	ofFill();
	ofSetColor(255, 255);
	ofDrawCircle(x, y, 0, r1);
	ofSetColor(c, 255);
	ofDrawCircle(x, y, 0, r2);
	ofSetColor(255, 255);
	ofDrawCircle(x, y, 0, r3);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	// Splash!

	if (key == ' ') splash.start();

	//--

	// Debug

#if defined(TARGET_WIN32)
	if (key == 'd')
	{
		splash.setToggleDebug();
	}
	if (key == OF_KEY_F3)
	{
		splash.setToggleModeFloating();
	}
	if (key == OF_KEY_F4)
	{
		splash.setToggleBorderless();
	}
	
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
#endif
}
