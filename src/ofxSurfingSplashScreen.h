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

	bool bDebug = false;
	bool bHandleInFront = true;
	bool bUseImageBorder = true;
	bool bSkipDrawNextFrame = false;//TODO: workaround to skip flick frame

	//--

	ofImage imageSplash;
	std::string path_Image = "-1";

	float alpha;
	float alphaBg;

	uint64_t tSplash;

	enum MySates
	{
		STATE_SPLASH_RUNNING = 0,
		STATE_SPLASH_FINISHED,//idle too
	};
	MySates appState;

	ofRectangle rBox;//used when non floating to draw into

	float _x, _y, _w, _h;//big/previous start call window shape.
	float _x_, _y_, _w_, _h_;//window shape when app startup is done.
	float wImg, hImg;//image shape

	bool bSplashing = false;
	bool bBgTransparent = true;
	bool bNoBorder = false;
	bool bDoneGetInitWindow = false;

	//--------------------------------------------------------------
	void doGetWindowState()
	{
		// Store previous state
		_x = ofGetWindowPositionX();
		_y = ofGetWindowPositionY();
		_w = ofGetWindowWidth();
		_h = ofGetWindowHeight();

		//TODO: fix
		//workaround to see top bar
		//_y += 28;

		//TODO: fix
		//workaround to correct offset
		_y -= 0;

		// To set window size as image
		wImg = imageSplash.getWidth();
		hImg = imageSplash.getHeight();

		//TODO:
		// fix workaround
		// weird offset 
		// it seems that below ofSetWindowShape do not passes the sizes correctly...
		wImg -= 16;
		hImg -= 39;
	}

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
			ofLogNotice("ofxSurfingSplashScreen") << "Image loaded.";
		else
			ofLogError("ofxSurfingSplashScreen") << "Image file not Found!";

		//--

		//start();
	};

	//--------------------------------------------------------------
	void startup()
	{
		////TODO:
		//// fix
		//restart();

		//--

		// Get and memorize initial window position and shape
		if (!bDoneGetInitWindow)
		{
			bDoneGetInitWindow = true;

			doGetWindowState();

			// init state
			_x_ = _x;
			_y_ = _y;
			_w_ = _w;
			_h_ = _h;
		}

		//--

		start();
	}

	//--

private:

	//--------------------------------------------------------------
	void update()
	{
		// Startup first frame
		if (ofGetFrameNum() == 0)
		{
			startup();
		}

		//--

		if (bDebug) {
			string s = ofToString(ofGetFrameRate(), 0) + " FPS " + getDebugInfo();
			ofSetWindowTitle(s);
		}

		//--

		// Elapsed time from the last start
		uint32_t t = ofGetElapsedTimeMillis() - tSplash;

		//--

		// Detects end!
		if (bSplashing && t >= tDuration)
		{
			// Finished! 
			// Done
			stop();
		}

		//--

		// Define a centered box to draw image
		if (!bModeFloating)
		{
			float xx = ofGetWidth() * 0.5f - imageSplash.getWidth() * 0.5f;
			float yy = ofGetHeight() * 0.5f - imageSplash.getHeight() * 0.5f;
			rBox = ofRectangle(xx, yy, imageSplash.getWidth(), imageSplash.getHeight());
		}

		//TODO: 
		// Image resize down
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

		//TODO: could be calculated to be finished exactly...
		float _dtBg = 0.05f;

		// calculate only once
		static bool b = false;
		if (!b) {
			b = true;
			int numFrames = ((0.2f * tDuration) / 1000.f) * 60.f;
			_dtBg = 1 / (float)numFrames;
			ofLogNotice("ofxSurfingSplashScreen") << "_dt:" << _dt;
			ofLogNotice("ofxSurfingSplashScreen") << "_dtBg:" << _dtBg;
		}

		//--

		// Update
		if (appState == STATE_SPLASH_RUNNING)
		{
			if (!bModeFloating)
			{
				if (t < tDuration * 0.3f) // Fading in
				{
					alpha += _dt;
					alpha = MIN(alpha, 1);
				}

				//--

				// Skip fade out when non floating mode 
				if (!bModeFloating)
				{
					if (t > tDuration * 0.7f) // Fading out
					{
						alpha -= _dt;
						alpha = MAX(alpha, 0);
					}

					if (t > tDuration * 0.8f) // Fading out Bg
					{
						alphaBg -= _dtBg;
						alphaBg = MAX(alphaBg, 0);
						ofLogNotice("ofxSurfingSplashScreen") << "alphaBg:" << alphaBg;
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

		//TODO: workaround to skip flick frame
		if (bSkipDrawNextFrame)
		{
			bSkipDrawNextFrame = false;
			return bSplashing;
		}

		//--

		uint32_t t = ofGetElapsedTimeMillis() - tSplash;

		if (bBgTransparent) drawBgTransparent();

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

				// Black
				ofSetColor(255, a - 25);
				ofNoFill();
				ofSetLineWidth(l);
				ofDrawRectangle(0, 0, rBox.getWidth(), rBox.getHeight());

				// White
				ofSetLineWidth(2.0f);
				ofSetColor(0, a - 25);
				ofDrawRectangle(-lh, -lh, rBox.getWidth() + l, rBox.getHeight() + l);
			}

			ofDisableAlphaBlending();
			ofPopMatrix();
			ofPopStyle();
		}

		//--

		if (bDebug)
		{
			string s = "";
			float x, y, w, h;
			x = 4;
			y = 14;
			s += ofToString(ofGetWindowPositionX()) + "," + ofToString(ofGetWindowPositionY());
			s += "\n";
			s += ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight());
			ofDrawBitmapStringHighlight(s, x, y);
			
			static ofBitmapFont f;
			float wf = f.getBoundingBox(s, 0, 0).getWidth() + 6;

			ofPushStyle();
			x = 0;
			y += 20;
			w = wf;
			//w = 40;
			h = 5;
			int p = 2;
			float v = ofMap(ofGetElapsedTimeMillis() - tSplash, 0, tDuration, 0, 1, true);
			ofFill();
			ofSetColor(0);
			ofDrawRectangle(x - p, y - p, w + 2 * p, h + 2 * p);
			ofSetColor(255);
			ofDrawRectangle(x, y, v * w, h);
			ofPopStyle();
		}

		return bSplashing;
	};

	//--------------------------------------------------------------
	void drawBgTransparent() { // To make the scene a bit darker behind.
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
		//--

		//TODO:
		// avoid to take sizes if is splashing..
		if (bModeFloating && !bSplashing) doGetWindowState();

		//--

		//workflow
		if (bSplashing) {
			stop();
			return;
		}

		//--

		alpha = 0.0f;
		alphaBg = 1.0f;
		appState = STATE_SPLASH_RUNNING;
		tSplash = ofGetElapsedTimeMillis();
		bSplashing = true;

		//--

		if (bModeFloating)
		{
			// Center
			float xx = ofGetScreenWidth() * 0.5 - wImg * 0.5;
			float yy = ofGetScreenHeight() * 0.5 - hImg * 0.5;

			ofSetWindowPosition(xx, yy);
			ofSetWindowShape(wImg, hImg);

#if defined(TARGET_WIN32)
			setBorderless(true);

			if (bHandleInFront)
			{
				// Make app always on top
				HWND W = GetActiveWindow();
				SetWindowPos(W, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
			}
#endif
		}
	};

	//--------------------------------------------------------------
	void stop()
	{
		bSplashing = false;
		appState = STATE_SPLASH_FINISHED;
		alpha = 0.0f;
		alphaBg = 0.0f;
		tSplash = 0;

		//--

		// Restore big/original size/pos window
		if (bModeFloating)
		{
			ofSetWindowShape(_w, _h);
			ofSetWindowPosition(_x, _y);

			// Enable border back
#if defined(TARGET_WIN32)
			setBorderless(false);

			if (bHandleInFront)
			{
				// Disable make app always on top
				HWND W = GetActiveWindow();
				SetWindowPos(W, HWND_NOTOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);
			}
#endif
			//TODO: workaround to skip flick frame
			bSkipDrawNextFrame = true;
		}
	};

	//TODO:
	// fix workaround for some startup troubles..
	//--------------------------------------------------------------
	void restart() {
		stop();
		start();
	};

	//--

	//--------------------------------------------------------------
	bool isSplashing()
	{
		return bSplashing;
	};

	//--------------------------------------------------------------
	string getDebugInfo()
	{
		string s = "  ";

		s += bModeFloating ? "FLOATING_TRUE " : "FLOATING_FALSE";
		s += "  ";
		s += "ALPHA:" + ofToString(alpha, 2);
		s += "  ";
		s += "ALPHA_BG:" + ofToString(alphaBg, 2);
		s += "  ";
		s += "WIN POS:" + ofToString(ofGetWindowPositionX()) + "," + ofToString(ofGetWindowPositionY());
		s += "  ";
		s += "SZ:" + ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight());

		return s;
	};
};