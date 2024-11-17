#pragma once

#include "ofMain.h"
#include "ofxEquiMapRender.h"

class ofApp : public ofBaseApp, public ofxEquiMapRender::Scene {
    ofxEquiMapRender::Renderer em;
    ofxEquiMapRender::CustomFboRenderer em2;

    std::vector<ofBoxPrimitive> boxes;
    std::vector<ofColor> colors; // Vetor para armazenar as cores

    bool bSaveEquiRect = false; // Variável para controlar o salvamento da imagem

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void drawEquiScene();
};