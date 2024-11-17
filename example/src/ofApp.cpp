#include "ofApp.h"

void ofApp::setup() {
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofClear(0);
    em.setup(1024, this);
    em2.setup(1024, this, GL_RGB, 4);
    ofSpherePrimitive p(1000, 24);
    m = p.getMesh();
}

void ofApp::drawEquiScene() {
    ofPushStyle();
    ofSetLineWidth(3);
    ofSetColor(192);
    m.drawWireframe();
    ofPopStyle();
}

void ofApp::update() {
    em.render();
    em2.render();
}

void ofApp::draw() {
    ofClear(0);
    if (ofGetKeyPressed(' ')) {
        em.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofDrawBitmapStringHighlight("Renderer", 10, 40);
    } else {
        em2.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofDrawBitmapStringHighlight("CustomFboRenderer", 10, 40);
    }

    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
}