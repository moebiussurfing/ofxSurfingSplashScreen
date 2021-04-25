#pragma once
#include "ofMain.h"

class SplashScreen
{
	//--

private:

	ofImage imageSplash;
	float splashAlpha;
	const int splashDuration = 4000;
	uint32_t splashtimer;
	enum MySates
	{
		STATE_PLASH_INTRO_STARTED = 0,
		STATE_PLASH_INTRO_ENDED,
	};
	MySates appSplashState;

	bool bSplashing;
	ofRectangle rBorder;
	bool bBlackTransparent = true;
	float splashAlphaBg;
	std::string path_Image = "-1";


	void mousePressed(ofMouseEventArgs &eventArgs)
	{
		if (isSplashing()) stop();
	}

	//--

public:

	SplashScreen()
	{
		//setup();

		ofAddListener(ofEvents().mousePressed, this, &SplashScreen::mousePressed);

	};

	~SplashScreen()
	{
		ofRemoveListener(ofEvents().mousePressed, this, &SplashScreen::mousePressed);
	};

	void setup(std::string path)
	{
		path_Image = path;
		setup();
	};

	void setup()
	{
		if (path_Image == "-1") imageSplash.load("banner.png"); // data/
		else imageSplash.load(path_Image);

		//imageSplash.load("../../../docs/itch.io/Paletto_Banner.png");

		start();
	};

	bool draw()
	{
		// update
		uint32_t te = ofGetElapsedTimeMillis() - splashtimer;

		// optimize doing return if not
		if (te > splashDuration) 
		{
			splashAlpha = splashAlphaBg = 0;
			bSplashing = false;
			return bSplashing;
		}

		//--
		
		//define rectangle		
		int xx = ofGetWidth() / 2 - imageSplash.getWidth() / 2;
		int yy = ofGetHeight() / 2 - imageSplash.getHeight() / 2;
		rBorder = ofRectangle(xx, yy, imageSplash.getWidth(), imageSplash.getHeight());

		//TODO: 
		//resize
		////fit max window size if image is bigger than window
		////resize rectangle
		//float ww = ofGetWidth();
		//float hh = ofGetHeight();
		//if (rBorder.getWidth()> ww)
		//{
		//	rBorder.setWidth(ww);
		//	rBorder.setHeight(ww * (imageSplash.getHeight()/ imageSplash.getWidth()));
		//}

		float dec = 0.1;
		//float dec = 0.05;
		//float dec = 1.0f / (60 / 2.0f);
		//splashDuration / 4 should be the max
		//float dec = (1.0f / 15) * (splashDuration/1000.0f);//15 frames are a seccond quarter

		if (bBlackTransparent) drawBlackTransparent();

		if (appSplashState == STATE_PLASH_INTRO_STARTED)
		{

			if (te > splashDuration) // finish
			{
				appSplashState = STATE_PLASH_INTRO_ENDED;
				bSplashing = false;
				splashAlpha = 0.0f;
			}
			else if (te < splashDuration * 0.3) // fade in
			{
				splashAlpha += dec;
				splashAlpha = MIN(splashAlpha, 1);
			}
			else if (te > splashDuration * 0.7) // fade out
			{
				splashAlpha -= dec;
				splashAlpha = MAX(splashAlpha, 0);
			}

			//-

			if (te > splashDuration * 0.8) // bg fade out
			{
				splashAlphaBg -= 0.05;
			}
		}

		//-

		// draw

		if (appSplashState == STATE_PLASH_INTRO_STARTED)
		{
			//splashAlpha = 1.0;
			ofPushStyle();
			ofPushMatrix();
			ofEnableAlphaBlending();

			ofTranslate(rBorder.getX(), rBorder.getY());
			//float a = ofWrap(sin(ofGetElapsedTimef()), 0, 1);

			//image
			int a = 255 * splashAlpha;
			ofSetColor(255, a);

			////fit max window size if image is bigger than window
			//float ww = ofGetWidth();
			//float hh = ofGetHeight();
			//if (ww > imageSplash.getWidth() || hh > imageSplash.getHeight()) {
			//	imageSplash.draw(0, 0, ww, hh);
			//}
			//else imageSplash.draw(0, 0);
			imageSplash.draw(0, 0);

			//border
			float l = 3.0f;
			float lh = l / 2.0f;

			//black
			ofSetColor(255, a - 25);
			ofNoFill();
			ofSetLineWidth(l);
			ofDrawRectangle(0, 0, rBorder.getWidth(), rBorder.getHeight());
			
			//white
			ofSetLineWidth(2.0f);
			ofSetColor(0, a - 25);
			ofDrawRectangle(-lh, -lh, rBorder.getWidth() + l, rBorder.getHeight() + l);

			ofDisableAlphaBlending();
			ofPopMatrix();
			ofPopStyle();
		}

		return bSplashing;
	};

	void drawBlackTransparent() {
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

	void start() {

		if (bSplashing) { stop(); return; }

		//moved to update
		//int xx = ofGetWidth() / 2 - imageSplash.getWidth() / 2;
		//int yy = ofGetHeight() / 2 - imageSplash.getHeight() / 2;
		//rBorder = ofRectangle(xx, xx, imageSplash.getWidth(), imageSplash.getHeight());

		//splash screen
		splashAlpha = 0.0f;
		splashAlphaBg = 1.0f;
		appSplashState = STATE_PLASH_INTRO_STARTED;
		splashtimer = ofGetElapsedTimeMillis();
		bSplashing = true;
	};

	void stop() {
		//TODO:
		uint32_t te = ofGetElapsedTimeMillis() - splashtimer;
		uint32_t tremain = splashDuration - te;

		appSplashState = STATE_PLASH_INTRO_ENDED;

		splashAlpha = 0.0f;
		splashAlphaBg = 0.0f;

		splashtimer = 0;
	}

	bool isSplashing()
	{
		return bSplashing;
	};
};