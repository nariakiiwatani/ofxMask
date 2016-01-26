#include "ofApp.h"
#include "ofxMask.h"

ofxMask mask_;
//--------------------------------------------------------------
void ofApp::setup(){
	mask_.allocate(300,300, ofxMask::LUMINANCE);
	mask_.beginMask();
	ofDrawCircle(100,100,50);
	ofDrawCircle(200,100,50);
	ofDrawCircle(200,200,50);
	ofDrawCircle(100,200,50);
	mask_.endMask();
	mask_.begin();
	ofDrawRectangle(100,100,100,100);
	mask_.end();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
	mask_.drawMasker();
	ofTranslate(300,0);
	mask_.drawMaskee();
	ofTranslate(300,0);
	mask_.draw();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
