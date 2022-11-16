#pragma once

#include "ofMain.h"

/*

	TODO

		fix drawing scene also when fading

*/

//--

// WINDOWS MANAGMENT
// https://gist.github.com/MartinBspheroid/8902181
// https://github.com/melak47/BorderlessWindow
// https://stackoverflow.com/questions/2398746/removing-window-border

//--

class ofxSurfingSplashScreen
{

private:

	// make splash floating
	bool bModeFloating = true;

	int splashDuration = 3000;//ms
	bool bDurationForced = false;

	bool bDebug = false;

	//--

	ofImage imageSplash;
	float splashAlpha;
	uint32_t splashtimer;
	enum MySates
	{
		STATE_SPLASH_RUNNING = 0,
		STATE_SPLASH_FINISHED,//idle too
	};
	MySates appSplashState;

	bool bSplashing = false;
	ofRectangle rBox;//used when non floating to draw into
	bool bBlackTransparent = true;
	float splashAlphaBg;
	std::string path_Image = "-1";

	float _x, _y, _w, _h;//original/big/previous window shape

	bool bNoBorder = false;
	bool bDoneGetInitWindow = false;

public:

#if defined(TARGET_WIN32)
	//--------------------------------------------------------------
	void setToggleBorderless() {
		bNoBorder = !bNoBorder;
		setBorderless(bNoBorder);
	}
#endif

	//--------------------------------------------------------------
	void setToggleModeFloating() {
		bModeFloating = !bModeFloating;

		if (!bDurationForced) {
			if (bModeFloating) splashDuration = 3000;
			else splashDuration = 4000;
		}
	}

	//--------------------------------------------------------------
	void setModeFloating(bool b) {//call before setup
		bModeFloating = b;

		if (!bDurationForced) {
			if (bModeFloating) splashDuration = 3000;
			else splashDuration = 4000;
		}
	}

	//--------------------------------------------------------------
	void setDuration(int t) { // in ms.
		splashDuration = t;
		bDurationForced = true;
	}

private:

#if defined(TARGET_WIN32)
	//--------------------------------------------------------------
	void setBorderless(bool b) {
		bNoBorder = b;

		// https://stackoverflow.com/questions/2398746/removing-window-border
		if (bNoBorder)
		{
			// Hide borders

			HWND AppWindow = GetActiveWindow();

			LONG lStyle = GetWindowLong(AppWindow, GWL_STYLE);
			lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
			SetWindowLong(AppWindow, GWL_STYLE, lStyle);

			LONG lExStyle = GetWindowLong(AppWindow, GWL_EXSTYLE);
			lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
			SetWindowLong(AppWindow, GWL_EXSTYLE, lExStyle);

			SetWindowPos(AppWindow, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
		}
		else
		{
			// Show borders

			HWND AppWindow = GetActiveWindow();

			LONG lStyle = GetWindowLong(AppWindow, GWL_STYLE);

			lStyle = WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

			SetWindowLong(AppWindow, GWL_STYLE, lStyle);

			LONG lExStyle = GetWindowLong(AppWindow, GWL_EXSTYLE);
			lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
			SetWindowLong(AppWindow, GWL_EXSTYLE, lExStyle);

			SetWindowPos(AppWindow, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE| SWP_NOZORDER | SWP_NOOWNERZORDER);

			ShowWindow(AppWindow, SW_SHOW);
		}
	};
#endif

	//--------------------------------------------------------------
	void mousePressed(ofMouseEventArgs& eventArgs)
	{
		if (isSplashing()) stop();
	}

	//--

public:

	//--------------------------------------------------------------
	ofxSurfingSplashScreen::ofxSurfingSplashScreen()
	{
		//setup();

		ofAddListener(ofEvents().mousePressed, this, &ofxSurfingSplashScreen::mousePressed);

	};

	//--------------------------------------------------------------
	ofxSurfingSplashScreen::~ofxSurfingSplashScreen()
	{
		ofRemoveListener(ofEvents().mousePressed, this, &ofxSurfingSplashScreen::mousePressed);
	};

	//--------------------------------------------------------------
	void setup(std::string path)
	{
		path_Image = path;
		setup();
	};

	//--------------------------------------------------------------
	void setup()
	{
		bool bloaded = false;
		if (path_Image == "-1") bloaded = imageSplash.load("banner.png"); // data/
		else bloaded = imageSplash.load(path_Image);

		//imageSplash.load("../../../docs/itch.io/Paletto_Banner.png");

		if (bloaded)
		{
			ofLogNotice("ofxSurfingSplashScreen") << "Image Loaded";
		}
		else ofLogError("ofxSurfingSplashScreen") << "Image Not Found!";

		//restart();
		start();
	};

	//private:
	//	//--------------------------------------------------------------
	//	void update()
	//	{
	//	}

public:
	string getDebugInfo() {
		string s = "  ";
		s += bModeFloating ? "FLOATING" : "NON FLOATING";
		//s += "  ";
		//s += bNoBorder ? "NO BRODER" : "BORDER";
		return s;
	}

	//--------------------------------------------------------------
	bool draw()
	{
		//TODO:
		// fix
		if (ofGetFrameNum() == 1) restart();

		// Update
		uint32_t te = ofGetElapsedTimeMillis() - splashtimer;

		// Optimize doing return if not
		if (bSplashing && te > splashDuration)
		{
			// Finished! Done
			bSplashing = false;
			splashAlpha = splashAlphaBg = 0;
			appSplashState = STATE_SPLASH_FINISHED;

			// Restore big/original size window
			if (bModeFloating)
			{
				ofSetWindowShape(_w, _h);
				ofSetWindowPosition(_x, _y);

#if defined(TARGET_WIN32)
				setBorderless(false);
#endif
			}

			return bSplashing;
		}

		//--

		// Define a centered box to draw image
		int xx = ofGetWidth() * 0.5 - imageSplash.getWidth() * 0.5;
		int yy = ofGetHeight() * 0.5 - imageSplash.getHeight() * 0.5;
		rBox = ofRectangle(xx, yy, imageSplash.getWidth(), imageSplash.getHeight());

		//TODO: 
		//resize
		////fit max window size if image is bigger than window
		////resize rectangle
		//float ww = ofGetWidth();
		//float hh = ofGetHeight();
		//if (rBox.getWidth()> ww)
		//{
		//	rBox.setWidth(ww);
		//	rBox.setHeight(ww * (imageSplash.getHeight()/ imageSplash.getWidth()));
		//}

		float dec = 0.1;
		//float dec = 0.05;
		//float dec = 1.0f / (60 / 2.0f);
		//splashDuration / 4 should be the max
		//float dec = (1.0f / 15) * (splashDuration/1000.0f);//15 frames are a seccond quarter

		if (bBlackTransparent) drawBlackTransparent();

		//--

		// Update
		if (appSplashState == STATE_SPLASH_RUNNING)
		{
			if (!bModeFloating)
			{
				if (te < splashDuration * 0.3) // fading in
				{
					splashAlpha += dec;
					splashAlpha = MIN(splashAlpha, 1);
				}

				if (!bModeFloating)//skip fade out on this mode 
				{
					if (te > splashDuration * 0.7) // fading out
					{
						splashAlpha -= dec;
						splashAlpha = MAX(splashAlpha, 0);
					}

					//--

					if (te > splashDuration * 0.8) // bg fading out
					{
						splashAlphaBg -= 0.05;
					}
				}
			}
		}

		//--

		// Draw

		if (appSplashState == STATE_SPLASH_RUNNING)
		{
			ofPushStyle();
			ofPushMatrix();
			ofEnableAlphaBlending();

			if (!bModeFloating) ofTranslate(rBox.getX(), rBox.getY());

			//float a = ofWrap(sin(ofGetElapsedTimef()), 0, 1);

			// Image
			int a;
			if (!bModeFloating) a = 255 * splashAlpha;
			else a = 255;
			ofSetColor(255, a);

			//TODO:
			//// Fit max window size if image is bigger than window
			//float ww = ofGetWidth();
			//float hh = ofGetHeight();
			//if (ww > imageSplash.getWidth() || hh > imageSplash.getHeight()) {
			//	imageSplash.draw(0, 0, ww, hh);
			//}
			//else imageSplash.draw(0, 0);

			// Original size
			imageSplash.draw(0, 0);

			//// Fix workaround 
			//// bc it seems that some spaces (top bar? borders?) are not being counted!
			// breaks aspect ratio!
			//// Force Fit
			//auto rView = ofGetCurrentViewport();
			//imageSplash.draw(rView);

			//--

			// Draw box borders
			if (!bModeFloating)
			{
				float l = 3.0f;
				float lh = l / 2.0f;

				// black
				ofSetColor(255, a - 25);
				ofNoFill();
				ofSetLineWidth(l);
				ofDrawRectangle(0, 0, rBox.getWidth(), rBox.getHeight());

				// white
				ofSetLineWidth(2.0f);
				ofSetColor(0, a - 25);
				ofDrawRectangle(-lh, -lh, rBox.getWidth() + l, rBox.getHeight() + l);
			}

			ofDisableAlphaBlending();
			ofPopMatrix();
			ofPopStyle();
		}

		if (bDebug)
		{
			string s = ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight());
			ofDrawBitmapStringHighlight(s, 4, 14);
		}

		return bSplashing;
	};

	//--------------------------------------------------------------
	void drawBlackTransparent() { // To make the scene a bit darker behind.
		if (isSplashing())
		{
			ofPushStyle();
			ofFill();
			ofSetColor(0, 240 * splashAlphaBg);
			ofRectangle r(0, 0, ofGetWidth(), ofGetHeight());
			ofDrawRectangle(r);
			ofPopStyle();
		}
	};

	//--------------------------------------------------------------
	void start()
	{
		if (bSplashing) { stop(); return; }

		// moved to update
		//int xx = ofGetWidth() / 2 - imageSplash.getWidth() / 2;
		//int yy = ofGetHeight() / 2 - imageSplash.getHeight() / 2;
		//rBox = ofRectangle(xx, xx, imageSplash.getWidth(), imageSplash.getHeight());

		// splashScreen screen
		splashAlpha = 0.0f;
		splashAlphaBg = 1.0f;
		appSplashState = STATE_SPLASH_RUNNING;
		splashtimer = ofGetElapsedTimeMillis();
		bSplashing = true;

		// set size as image
		// store pre
		if (bModeFloating)
		{
			// get and memorize init window position and shape
			if (!bDoneGetInitWindow)
			{
				bDoneGetInitWindow = true;

				_x = ofGetWindowPositionX();
				_y = ofGetWindowPositionY();
				//_y += 28;//workaround to see top bar
				_w = ofGetWindowWidth();
				_h = ofGetWindowHeight();
			}

			float wi = imageSplash.getWidth();
			float hi = imageSplash.getHeight();

			//TODO:
			// fix workaround
			// weird offset 
			wi -= 16;
			hi -= 39;

			ofSetWindowPosition(ofGetScreenWidth() * 0.5 - wi * 0.5, ofGetScreenHeight() * 0.5 - hi * 0.5);//center
			ofSetWindowShape(wi, hi);

#if defined(TARGET_WIN32)
			setBorderless(true);
#endif
		}
	};

	//--------------------------------------------------------------
	void stop()
	{
		////TODO:
		//uint32_t te = ofGetElapsedTimeMillis() - splashtimer;
		//uint32_t tremain = splashDuration - te;

		appSplashState = STATE_SPLASH_FINISHED;

		splashAlpha = 0.0f;
		splashAlphaBg = 0.0f;

		splashtimer = 0;
	}

	//--------------------------------------------------------------
	bool isSplashing()
	{
		return bSplashing;
	};

	//TODO:
	// fix workaround for some startup troubles..
	//--------------------------------------------------------------
	void restart() {
		stop();
		bSplashing = false;
		start();
	}
};