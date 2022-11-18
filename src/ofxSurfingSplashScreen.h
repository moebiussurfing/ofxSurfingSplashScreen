#pragma once

#include "ofMain.h"

/*

	TODO

		fix drawing scene also when fading

*/

//--

// Windows management for WIN32
// https://gist.github.com/MartinBspheroid/8902181
// https://github.com/melak47/BorderlessWindow
// https://stackoverflow.com/questions/2398746/removing-window-border

//--

#define AUTO_CASE_CREATE(a) case a: return #a

namespace ofxSurfingSplash {

	/* Return a linear value in range [0,1] every delay (in seconds). */
	inline float Tick(float delay = 1.0f) {
		return fmodf(ofGetElapsedTimeMillis() / 1000.0f, delay) / delay;
	}
	/* Return a linear value in range [0,1] every delay (in seconds),
	 with value equal 0 at boundaries, and 1 at delay / 2. */
	inline float Bounce(float delay = 1.0f) {
		return 0.5f * (1.0 + glm::sin(Tick(delay) * glm::two_pi<float>()));
	}
	/* Noise function used by the gradient scaling. */
	inline float Noise(const ofPoint& vertex = ofPoint(1, -1)) {
		return ofNoise(0.05f * vertex + 0.5f * ofGetElapsedTimeMillis() * 0.002f);
	}
}

class ofxSurfingSplashScreen
{

private:


private:

	// make splash floating
	bool bModeFloating = true;

	int tDuration = 3000;//ms
	bool bDurationForced = false;

	// related to duration
	float tIn = 0.08f;//fade in ends
	float tOut1 = 0.6f;//fade out starts. duration will be the same than fade in!
	float tOut2 = 0.8f;//fade out Bg starts

	bool bDebug = false;
	bool bHandleInFront = true;//allows app to front
	bool bSkipDrawNextFrame = false;//TODO: workaround to skip flick frame

	bool bUseImageBorder = true;//draws two borders in image box 
	float borderLineSz = 2.0f;
	ofColor cb1 = ofColor::white;
	ofColor cb2 = ofColor::black;

	int alphaBgMax = 215;

	//--

	ofImage imageSplash;
	std::string path_Image = "-1";

	float alpha;
	float alphaBg;

	uint64_t tSplash;

	enum SPLASH_STATES
	{
		SPLASH_IDDLE = 0,
		SPLASH_STARTED_IN,
		SPLASH_FADING_OUT,
		SPLASH_FADING_OUT_BG,
		SPLASH_LATCH
	};
	SPLASH_STATES appState = SPLASH_IDDLE;

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

		////TODO:
		//// fix workaround
		//// weird offset 
		//// it seems that below ofSetWindowShape do not passes the sizes correctly...
		//wImg -= 16;
		//hImg -= 39;
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
	void setBorderBox(bool b) {//extra box border.
		bUseImageBorder = b;
	}

	//--------------------------------------------------------------
	void setDebug(bool b) {
		bDebug = b;
		//if (!bDebug) ofSetWindowTitle("");
	}
	//--------------------------------------------------------------
	void setToggleDebug() {
		bDebug = !bDebug;
		//if (!bDebug) ofSetWindowTitle("");
	}

	//--------------------------------------------------------------
	void setColorBorder1(ofColor c) {
		cb1 = c;
		bUseImageBorder = true;
	}
	//--------------------------------------------------------------
	void setColorBorder2(ofColor c) {
		cb2 = c;
		bUseImageBorder = true;
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
		if (isSplashing()) stopFaded();
		//if (isSplashing()) stop();
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
		//-- 

		// Startup first frame
		if (ofGetFrameNum() == 0)
		{
			startup();
		}

		//--

		// Elapsed time from the last start
		uint32_t t = ofGetElapsedTimeMillis() - tSplash;

		//--

		// Fading speeds
		float _tf = (1000 * ofGetLastFrameTime());//frame duration
		float _dti = 1.f / ((tDuration * tIn) / _tf);//fade in duration
		float _dtBg = 1.f / ((tDuration * (1 - tOut2)) / _tf);//fade out bg duration

		//--

		if (appState == SPLASH_LATCH)
		{
			alpha = 1.0f;
			alphaBg = 1.0f;
			t = tDuration * tOut1;//locked time 

			//return;
		}

		//--

		// Detects end!
		// For any appState
		if (bSplashing && t >= tDuration)
		{
			// Finished! 
			// Done
			stop();
			// appState = SPLASH_IDDLE;
			//ofLogNotice("ofxSurfingSplashScreen") << "STATE:" << stringForState(appState);
		}

		//--

		else if (appState == SPLASH_STARTED_IN)
		{
			if (!bModeFloating)//fades are disabled on floating mode!
			{
				if (t < tDuration * tIn) // Fading in
				{
					alpha += _dti;
					alpha = MIN(alpha, 1);
				}
			}

			//--

			// Skip fade out when non floating mode 
			if (!bModeFloating)
			{
				if (t >= tDuration * tOut1) // Fading out
				{
					appState = SPLASH_FADING_OUT;
					ofLogNotice("ofxSurfingSplashScreen") << "STATE:" << stringForState(appState);
					if (alpha < 1) alpha = 1;
				}
			}
		}

		//--

		else if (appState == SPLASH_FADING_OUT)
		{
			if (!bModeFloating)
			{
				if (alpha > 0)
				{
					alpha -= _dti;
					alpha = MAX(alpha, 0);
					ofLogNotice("ofxSurfingSplashScreen") << "alpha:" << alpha;
				}
			}

			if (!bModeFloating)
			{
				if (t >= tDuration * tOut2) // Fading out Bg
				{
					appState = SPLASH_FADING_OUT_BG;
					ofLogNotice("ofxSurfingSplashScreen") << "STATE:" << stringForState(appState);
				}
			}
		}

		//--

		else if (appState == SPLASH_FADING_OUT_BG)
		{
			//TODO: fix workaround
			// we run fade out here too in this state,
			// bc maybe first fade is not finished, 
			// then both fades can run in parallel.
			if (!bModeFloating)
			{
				if (alpha > 0)
				{
					alpha -= _dti;
					alpha = MAX(alpha, 0);
					ofLogNotice("ofxSurfingSplashScreen") << "alpha:" << alpha;
				}
			}

			if (!bModeFloating)
			{
				alphaBg -= _dtBg;
				alphaBg = MAX(alphaBg, 0);
				ofLogNotice("ofxSurfingSplashScreen") << "alphaBg:" << alphaBg;

				// Finished
				if (t >= tDuration)
				{
					// Finished! 
					// Done
					stop();
					//appState = SPLASH_IDDLE;
					//ofLogNotice("ofxSurfingSplashScreen") << "STATE:" << stringForState(appState);
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

		//TODO: workaround to skip flick frame
		if (bSkipDrawNextFrame)
		{
			bSkipDrawNextFrame = false;

			return bSplashing;
		}

		//--

		if (bBgTransparent) drawBgTransparent();

		//--

		uint32_t t = ofGetElapsedTimeMillis() - tSplash;

		if (appState == SPLASH_LATCH)
		{
			t = tDuration * tOut1;//locked time 
		}

		//--

		if (appState == SPLASH_STARTED_IN ||
			appState == SPLASH_FADING_OUT ||
			appState == SPLASH_FADING_OUT_BG ||
			appState == SPLASH_LATCH)
		{
			ofPushStyle();
			ofPushMatrix();
			//ofEnableAlphaBlending();
			{
				//--

				// Define a centered box to draw image
				//if (!bModeFloating)
				{
					float xx = ofGetWidth() * 0.5f - wImg * 0.5f;
					float yy = ofGetHeight() * 0.5f - hImg * 0.5f;
					rBox = ofRectangle(xx, yy, wImg, hImg);
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

				if (!bModeFloating) ofTranslate(rBox.getX(), rBox.getY());

				int a;
				if (!bModeFloating) a = 255 * alpha;
				else a = 255;

				//fades are disabled on floating mode!
				// //a = 255 * alpha;
				//if (bModeFloating) ofClear(0, 255);

				ofSetColor(255, a);

				//--

				// Draw Image

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
				//if (!bModeFloating && bUseImageBorder)
				if (bUseImageBorder)
				{
					float lh = borderLineSz / 2.0f;

					ofNoFill();

					// Blink
					float _a = ofMap(ofxSurfingSplash::Bounce(.75), 0, 1, 0.65, 1, true);

					// Black
					int _a2 = cb2.a * _a;
					if (!bModeFloating) _a2 = ofClamp(cb2.a * alpha * _a, 0, 225);
					ofSetColor(cb2, _a2);
					ofSetLineWidth(borderLineSz);
					if (bModeFloating)
						ofDrawRectangle(lh, lh, wImg - borderLineSz, hImg - borderLineSz);//inner
					else
						ofDrawRectangle(0, 0, rBox.getWidth(), rBox.getHeight());//outer

					// White
					int _a1 = cb1.a * _a;
					if (!bModeFloating) _a1 = ofClamp(cb1.a * alpha * _a, 0, 225);
					ofSetColor(cb1, _a1);
					ofSetLineWidth(borderLineSz);
					if (bModeFloating)
						ofDrawRectangle(borderLineSz, borderLineSz, wImg - 2 * borderLineSz, hImg - 2 * borderLineSz);//inner
					else
						ofDrawRectangle(-lh, -lh, rBox.getWidth() + borderLineSz, rBox.getHeight() + borderLineSz);//outer
				}
			}
			//ofDisableAlphaBlending();
			ofPopMatrix();
			ofPopStyle();
		}

		//--

		if (bDebug)
		{
			drawDebug();
		}

		//--

		return bSplashing;
	};

	//--------------------------------------------------------------
	void drawDebug()
	{
		uint32_t t = ofGetElapsedTimeMillis() - tSplash;

		if (appState == SPLASH_LATCH)
		{
			t = tDuration * tOut1;//locked time 
		}

		//--

		float x, y, w, h;

		ofPushStyle();
		//ofEnableAlphaBlending();

		// Progress bar
		x = 0;
		y = 2;
		//y += 2;
		if (bModeFloating) w = wImg;
		else w = ofGetWidth();
		h = 5;
		int p = 2;
		int p2 = 3;
		float v = ofMap(t, 0, tDuration, 0, 1, true);
		ofFill();
		ofSetColor(0);
		ofDrawRectangle(x - p, y - p, w + 2 * p, h + 2 * p);
		ofSetColor(255);
		ofDrawRectangle(x, y, v * w, h);

		// Time marks
		ofSetColor(ofColor::red);
		ofNoFill();
		float xx;
		float yy = y + 21;
		string ss;
		xx = w * tIn;
		ofDrawLine(xx, y, xx, y + h);
		ss = ofToString(tDuration * tIn, 0) + "ms";
		ofDrawBitmapStringHighlight(ss, xx + p2, yy + 19);
		xx = w * tOut1;
		ofDrawLine(xx, y, xx, y + h);
		ss = ofToString(tDuration * tOut1, 0) + "ms";
		ofDrawBitmapStringHighlight(ss, xx + p2, yy);
		xx = w * tOut2;
		ofDrawLine(xx, y, xx, y + h);
		ss = ofToString(tDuration * tOut2, 0) + "ms";
		ofDrawBitmapStringHighlight(ss, xx + p2, yy);
		ss = ofToString(tDuration, 0) + "ms";
		static ofBitmapFont f;
		auto ww = f.getBoundingBox(ss, 0, 0).getWidth();
		xx = w - ww - 7;
		ofDrawBitmapStringHighlight(ss, xx + p2, yy);

		// Text info
		x = 4;
		y += h;
		y += 15;
		string s = ofToString(ofGetFrameRate(), 0) + ":FPS " + getDebugInfo();

		// left
		ofDrawBitmapStringHighlight(s, x, y, ofColor(0, 255), ofColor(255, 255));

		//// right
		//static ofBitmapFont f;
		//auto ww = f.getBoundingBox(s, 0, 0).getWidth() + 4;
		//ofDrawBitmapStringHighlight(s, ofGetWidth() - ww, y, ofColor(0, 255), ofColor(255, 255));

		ofPopStyle();
	};

	//--------------------------------------------------------------
	void drawBgTransparent() { // To make the scene a bit darker behind.
		if (isSplashing())
		{
			ofPushStyle();
			ofFill();
			ofSetColor(0, alphaBgMax * alphaBg);
			ofRectangle r(0, 0, ofGetWidth(), ofGetHeight());
			ofDrawRectangle(r);
			ofPopStyle();
		}
	};

	//--------------------------------------------------------------
	void doToggleLatch()
	{
		//if (appState == SPLASH_LATCH)

		if (appState == SPLASH_IDDLE)
		{
			// prepare
			start();

			// override force
			//alpha = 1.0f;
			//alphaBg = 1.0f;
			//tSplash = ofGetElapsedTimeMillis();//not used
			//bSplashing = true;
			appState = SPLASH_LATCH;
			ofLogNotice("ofxSurfingSplashScreen") << "STATE:" << stringForState(appState);
		}
		else
		{
			// instant
			//stop();

			// faded
			stopFaded();
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
		if (bSplashing)
		{
			stop();
			return;
		}

		//--

		alpha = 0.0f;
		alphaBg = 1.0f;
		tSplash = ofGetElapsedTimeMillis();
		bSplashing = true;
		appState = SPLASH_STARTED_IN;
		ofLogNotice("ofxSurfingSplashScreen") << "STATE:" << stringForState(appState);

		//--

		if (bModeFloating)
		{
			// Center
			float xx = ofGetScreenWidth() * 0.5 - wImg * 0.5;
			float yy = ofGetScreenHeight() * 0.5 - hImg * 0.5;

			ofSetWindowPosition(xx, yy);

			//ofSetWindowShape(wImg, hImg);
			//TODO:
			// fix workaround
			// weird offset 
			// it seems that below ofSetWindowShape do not passes the sizes correctly...
			ofSetWindowShape(wImg - 16, hImg - 39);

			// Disable border
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
	void stopFaded()
	{
		stop();

		alpha = 1.0f;
		alphaBg = 1.0f;

		appState = SPLASH_FADING_OUT;
		tSplash = ofGetElapsedTimeMillis() + tDuration * tOut1;

		//appState = SPLASH_FADING_OUT_BG;
		//tSplash = ofGetElapsedTimeMillis() + (tDuration * tOut2);

		ofLogNotice("ofxSurfingSplashScreen") << "STATE:" << stringForState(appState);
	};

	//--------------------------------------------------------------
	void stop()
	{
		appState = SPLASH_IDDLE;

		ofLogNotice("ofxSurfingSplashScreen") << "STATE:" << stringForState(appState);

		bSplashing = false;
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

		s += "Pos:" + ofToString(ofGetWindowPositionX()) + "," + ofToString(ofGetWindowPositionY());
		s += "  ";
		s += "Sz:" + ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight());
		s += "  ";
		s += bModeFloating ? "   FLOATING" : "NO_FLOATING";
		s += "  ";
		if (!bModeFloating)
		{
			s += "Alpha:" + ofToString(alpha, 2);
			s += "  ";
			s += "Bg:" + ofToString(alphaBg, 2);
			s += "  ";
		}
		//s += "  ";
		//s += bSplashing ? "SPLASHING" : "         ";
		s += "#" + ofToString(appState) + "_" + stringForState(appState);
		return s;
	};

	//--------------------------------------------------------------
	std::string stringForState(SPLASH_STATES m) {
		switch (m) {
			AUTO_CASE_CREATE(SPLASH_IDDLE);
			AUTO_CASE_CREATE(SPLASH_STARTED_IN);
			AUTO_CASE_CREATE(SPLASH_FADING_OUT);
			AUTO_CASE_CREATE(SPLASH_FADING_OUT_BG);
			AUTO_CASE_CREATE(SPLASH_LATCH);
		default: return "ERROR!";
		}
	};
};