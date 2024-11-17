#pragma once

#include "ofMain.h"
#include "ofxEquiMapRender.h"

class ofApp : public ofBaseApp, public ofxEquiMapRender::Scene {
    ofxEquiMapRender::Renderer em;
    ofxEquiMapRender::CustomFboRenderer em2;

    std::vector<ofBoxPrimitive> boxes;
    std::vector<ofColor> colors; // Vetor para armazenar as cores
    ofVboMesh m;

public:
    void setup();
    void update();
    void draw();
    void drawEquiScene();
};