# ofxSurfingSplashScreen
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Overview
An **openFrameworks** addon to trig a faded out splash image. Typically when starting an *OF App*.

## Screencast
![Alt text](readme_images/Capture1.gif?raw=true "moebiusSurfing")

## Features
- NOT multi-threading nor drawing during **ofApp::setup()** for "longer loading" apps.
- Faded Out.
- Can be trigged by code.
- Can disable all other drawings durings splash.

## Usage

* Look example.

```	
#include "SplashScreen.h"
SplashScreen splash;

void ofApp::setup(){
	// pick your image
	splash.setup("assets/images/Paletto_Banner.png");
}

void ofApp::draw(){
{
	if (splash.draw()) return;
	//can skip other drawing during splash
}

// manual trig splash
	splash.start();
```

## TODO:
* Rescale drawing when image is bigger than the window.
* Add multithreading to allow drawing during slow setups.  
[ Help on this is welcome! ]

## Tested systems
* **Windows 10** / **VS 2017** / **OF ~0.11**

## Author
An addon by **@moebiusSurfing**  
*( ManuMolina ) 2021*  

## License
**MIT License**