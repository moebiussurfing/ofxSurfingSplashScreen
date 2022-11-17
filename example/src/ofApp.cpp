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
	//splash.setModeFloating(false);

	splash.setDebug(true);

	//--

	splash.setup("assets/images/Paletto_Banner.png");
	//splash.setup("assets/images/Aldous.png");
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofClear(c1);

	if (splash.draw()) return;
	// Skipped during splashing

	//--

	drawScene();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	float sz = max(ofGetWidth(), ofGetHeight());
	float x = ofGetWidth() * 0.5f;
	float y = ofGetHeight() * 0.5f;
	int a = 255;

	// a.
	//const int d = 30;
	//int f = ofGetFrameNum() % d;
	//float r0 = ofMap(f, 0, d, 0, 1, true);

	// b.
	float r0 = 0.75 + Bounce();
	//float r0 = Noise() * 0.75 + Bounce();
	//float r0 = Noise();

	/*
	{
		ofPushStyle();
		ofFill();

		float r1 = r0 * sz;
		float r2 = r0 * sz * 0.2;
		float r3 = r0 * sz * 0.025;
		float r4 = r0 * sz * 0.005;

		ofSetColor(c2, a);
		ofDrawCircle(x, y, 0, r1);
		ofSetColor(c1, a);
		ofDrawCircle(x, y, 0, r2);
		ofSetColor(c2, a);
		ofDrawCircle(x, y, 0, r3);
		ofSetColor(c1, a);
		ofDrawCircle(x, y, 0, r4);

		ofPopStyle();
	}
	*/

	///*
	{
		ofPushStyle();
		ofFill();

		//int amt = 100 * Noise();
		int amt = 10;
		float _d = 1;
		float d0 = 1.f / (float)amt;
		//float d0 = 1.f / ((float)amt * Noise());

		for (int i = 0; i < amt; i++)
		{
			_d = _d - d0;
			float _r = r0 * sz * _d;

			bool _b = (i % 2 == 0);//odd/even
			ofSetColor(_b ? c1 : c2, a);
			ofDrawCircle(x, y, 0, _r);
		}

		ofPopStyle();
	}
	//*/
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
