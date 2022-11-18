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

	// Customize
	if (1) // <- Put to 1 or 0
	{
		splash.setDebug(true);

		//splash.setDuration(4000); 

		// No floating mode (false) 
		// means window is not resized nor border less control!
		splash.setModeFloating(false);

		splash.setColorBorder1(ofColor(c1, 100));
		splash.setColorBorder2(ofColor(ofColor::turquoise, 70));
	}

	//--

	splash.setup("assets/images/Paletto_Banner.png");
	//splash.setup("assets/images/Aldous.png");
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofClear(c1);

	// B.
	// Draw under (before) the Scene
	//if (splash.draw()) return;
	//// Skipped during splashing

	//--

	drawScene();

	//--

	// A.
	// Draw on top (after) the Scene
	splash.draw();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	float sz = max(ofGetWidth(), ofGetHeight());
	float x = ofGetWidth() * 0.5f;
	float y = ofGetHeight() * 0.5f;
	int a = 255;

	// kind of sinus 
	float r0 = 0.75 + 0.5f * (1.0 + glm::sin(fmodf(ofGetElapsedTimeMillis() / 1000.0f, 1.f)
		/ 1.f * glm::two_pi<float>()));

	ofPushStyle();
	ofFill();

	int amt = 10;
	float _d = 1;
	float d0 = 1.f / (float)amt;

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

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	// Splash with timer to hide!
	if (key == ' ') splash.start();

	// Splash latched! 
	// Mouse click or trig again to hide!
	if (key == OF_KEY_RETURN) splash.doToggleLatch();

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
