# ofxSurfingSplashScreen
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Stage: beta](https://img.shields.io/badge/-alpha-red)

## Overview
ofxSurfingSplashScreen is an addon to trig a faded out splash image.

## Screenshot
![Alt text](/Capture1.gif?raw=true "moebiusSurfing")

## Features
- Faded Out.
- Not multi-threading nor drawing during **ofApp::setup()** for "longer loading" apps.
- Can be trigged by code.
- Can disable all other drawings durings splash.

## Usage

```		
void ofApp::setup(){
	// pick your image
	splash.setup("assets/images/Paletto_Banner.png");
}

void ofApp::draw(){
{
	if (splash.draw()) return;
	//skip other drawing during splash

	//..
}

// manual trig splash
	splash.start();
```

## Tested systems
* **Windows 10** / **VS 2017** / **OF ~0.11**

## Author
An addon by **@moebiusSurfing**  
*( ManuMolina ) 2021*  

<br/>

## License
[**MIT License**](https://github.com/LICENSE)