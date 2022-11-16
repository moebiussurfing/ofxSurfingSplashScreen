#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	splash.setup("assets/images/Paletto_Banner.png");

	HWND m_hWnd = WindowFromDC(wglGetCurrentDC());
	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);
	style &= ~WS_DLGFRAME;
	style &= ~WS_CAPTION;
	style &= ~WS_BORDER;
	style &= WS_POPUP;

	LONG exstyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	exstyle &= ~WS_EX_DLGMODALFRAME;

	::SetWindowLong(m_hWnd, GWL_STYLE, style);
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, exstyle);

	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofClear(ofColor::blue);

	if (splash.draw()) return;

	//skipped during splashing
	ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, 100, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	// splash
	splash.start();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
