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

	int tDuration = 3000;//ms
	bool bDurationForced = false;

	bool bDebug = 0;
	bool bUseImageBorder = true;

	//--

	ofImage imageSplash;
	std::string path_Image = "-1";

	float alpha;

	uint64_t tSplash;

	enum MySates
	{
		STATE_SPLASH_RUNNING = 0,
		STATE_SPLASH_FINISHED,//idle too
	};
	MySates appState;

	ofRectangle rBox;//used when non floating to draw into
	float alphaBg;

	float _x, _y, _w, _h;//original/big/previous window shape

	bool bSplashing = false;
	bool bBlackTransparent = true;
	bool bNoBorder = false;
	bool bDoneGetInitWindow = false;

public:

	//--------------------------------------------------------------
	void setDuration(int t) { // in ms.
		tDuration = t;
		bDurationForced = true;
	}

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
			if (bModeFloating) tDuration = 3000;
			else tDuration = 4000;
		}
	}

	//--------------------------------------------------------------
	void setModeFloating(bool b) { // call before setup!
		bModeFloating = b;

		if (!bDurationForced) {
			if (bModeFloating) tDuration = 3000;
			else tDuration = 4000;
		}
	}

	//--------------------------------------------------------------
	void setBorderBox(bool b) {//disable extra box border. not the window border. for non floating mode
		bUseImageBorder = b;
	}

	//--------------------------------------------------------------
	void setDebug(bool b) {
		bDebug = b;
		if (!bDebug) ofSetWindowTitle("");
	}
	//--------------------------------------------------------------
	void setToggleDebug() {
		bDebug = !bDebug;
		if (!bDebug) ofSetWindowTitle("");
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

			HWND W = GetActiveWindow();

			LONG lStyle = GetWindowLong(W, GWL_STYLE);
			lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
			SetWindowLong(W, GWL_STYLE, lStyle);

			LONG lExStyle = GetWindowLong(W, GWL_EXSTYLE);
			lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
			SetWindowLong(W, GWL_EXSTYLE, lExStyle);

			SetWindowPos(W, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
		}
		else
		{
			// Show borders

			HWND W = GetActiveWindow();

			LONG lStyle = GetWindowLong(W, GWL_STYLE);
			lStyle = WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

			SetWindowLong(W, GWL_STYLE, lStyle);

			LONG lExStyle = GetWindowLong(W, GWL_EXSTYLE);
			lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
			SetWindowLong(W, GWL_EXSTYLE, lExStyle);

			SetWindowPos(W, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);

			ShowWindow(W, SW_SHOW);
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

		//path_Image = "../../../docs/itch.io/Paletto_Banner.png";

		if (path_Image == "-1")
			bloaded = imageSplash.load("banner.png");
		// data/banner.png default file. dont requires to call setup()
		else
			bloaded = imageSplash.load(path_Image);

		if (bloaded)
			ofLogNotice("ofxSurfingSplashScreen") << "Image Loaded";
		else
			ofLogError("ofxSurfingSplashScreen") << "Image Not Found!";

		start();
	};

	//--

private:

	//--------------------------------------------------------------
	void update()
	{
		//TODO:
		// fix
		if (ofGetFrameNum() == 1) restart();

		if (bDebug) {
			string s = ofToString(ofGetFrameRate(), 0) + " FPS " + getDebugInfo();
			ofSetWindowTitle(s);
		}

		//--

		uint32_t t = ofGetElapsedTimeMillis() - tSplash;

		//--

		// Optimize doing return if not
		if (bSplashing && t >= tDuration)
		{
			// Finished! Done
			stop();

			//--

			// Restore big/original size/pos window
			if (bModeFloating)
			{
				ofSetWindowShape(_w, _h);
				ofSetWindowPosition(_x, _y);

				// enable border back
#if defined(TARGET_WIN32)
				setBorderless(false);

				// Disable make app always on top
				HWND W = GetActiveWindow();
				SetWindowPos(W, HWND_NOTOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
#endif
			}
		}

		//--

		// Define a centered box to draw image
		if (!bModeFloating)
		{
			int xx = ofGetWidth() * 0.5 - imageSplash.getWidth() * 0.5;
			int yy = ofGetHeight() * 0.5 - imageSplash.getHeight() * 0.5;
			rBox = ofRectangle(xx, yy, imageSplash.getWidth(), imageSplash.getHeight());
		}

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

		//--

		// Fading speed
		float _dt = 0.1f;
		//float _dt = 0.05;
		//float _dt = 1.0f / (60 / 2.0f);
		//tDuration / 4 should be the max
		//float _dt = (1.0f / 15) * (tDuration/1000.0f);
		//15 frames are a second quarter

		//--

		// Update
		if (appState == STATE_SPLASH_RUNNING)
		{
			if (!bModeFloating)
			{
				if (t < tDuration * 0.3) // Fading in
				{
					alpha += _dt;
					alpha = MIN(alpha, 1);
				}

				//--

				// Skip fade out when non floating mode 
				if (!bModeFloating)
				{
					if (t > tDuration * 0.7) // fading out
					{
						alpha -= _dt;
						alpha = MAX(alpha, 0);
					}

					if (t > tDuration * 0.8) // fading out bg
					{
						alphaBg -= 0.05;
						alphaBg = MAX(alphaBg, 0);
					}
				}
			}
		}
	}

	//--

public:

	//--------------------------------------------------------------
	bool draw()
	{
		update();

		//--

		uint32_t t = ofGetElapsedTimeMillis() - tSplash;

		if (bBlackTransparent) drawBlackTransparent();

		if (appState == STATE_SPLASH_RUNNING)
		{
			ofPushStyle();
			ofPushMatrix();
			ofEnableAlphaBlending();

			if (!bModeFloating) ofTranslate(rBox.getX(), rBox.getY());

			//float a = ofWrap(sin(ofGetElapsedTimef()), 0, 1);

			// Image
			int a;
			if (!bModeFloating) a = 255 * alpha;
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
			if (!bModeFloating && bUseImageBorder)
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
			ofSetColor(0, 240 * alphaBg);
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
		alpha = 0.0f;
		alphaBg = 1.0f;
		appState = STATE_SPLASH_RUNNING;
		tSplash = ofGetElapsedTimeMillis();
		bSplashing = true;

		//--

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

			// Make app always on top
			HWND W = GetActiveWindow();
			SetWindowPos(W, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
#endif
		}
	};

	//--------------------------------------------------------------
	void stop()
	{
		////TODO:
		//uint32_t t = ofGetElapsedTimeMillis() - tSplash;
		//uint32_t tremain = tDuration - t;

		bSplashing = false;
		appState = STATE_SPLASH_FINISHED;
		alpha = 0.0f;
		alphaBg = 0.0f;
		tSplash = 0;
	};

	//TODO:
	// fix workaround for some startup troubles..
	//--------------------------------------------------------------
	void restart() {
		stop();
		start();
	};

	//--------------------------------------------------------------
	bool isSplashing()
	{
		return bSplashing;
	};

	//--------------------------------------------------------------
	string getDebugInfo()
	{
		string s = "  ";

		s += bModeFloating ? "FLOATING" : "NON FLOATING";
		s += "  ";
		s += "alpha:" + ofToString(alpha, 2);
		s += "  ";
		s += "alphaBg:" + ofToString(alphaBg, 2);
		s += "  ";
		s += "window:" + ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight());

		return s;
	};
};