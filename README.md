# ofxSurfingSplashScreen
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Overview
An **openFrameworks** addon to trig a faded out splash image. Typically when starting an `oF_App`.

## Screencast
![](ofxSurfingSplashScreen.gif)

## Features
- Floating window and borderless mode. (WIN32 Only)
- Faded Out on non Floating Window mode.
- Can be trigged by code during runtime.
- Can disable all other app drawing during splash.
- NOT multi-threading nor drawing during `setup()` for "longer loading" apps.

## Usage

* Look example.

```.cpp	
#include "ofxSurfingSplashScreen.h"
ofxSurfingSplashScreen splash;

void ofApp::setup(){
	// Pick your image
	splash.setup("assets/images/Paletto_Banner.png");
}

void ofApp::draw(){
{
	if (splash.draw()) return;
	// Can skip other drawing during splash happens

	/* Draw your scene */
}

// Manual trig splash
splash.start();
```

## TODO:
* Fix small flick when transition window sizes on floating mode.
* Rescale drawing when image is bigger than the window.
* Add multithreading to allow drawing during slow setups.
* Add transparent window using PNGs with alpha.  

## Tested Systems
* **Windows 10** / **VS 2017** / **OF ~0.11**

## Author
An addon by **@moebiusSurfing**  
*( ManuMolina ) 2022*  

## License
**MIT License**