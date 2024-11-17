#include "ofApp.h"

void ofApp::setup() {
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofClear(0);
    em.setup(1024, this);
    em2.setup(1024, this, GL_RGB, 4);

    // Cria 200 cubos com posições e tamanhos aleatórios
    for (int i = 0; i < 30; i++) {
        float size = ofRandom(50, 200);
        float x = ofRandom(-500, 500);
        float y = ofRandom(-300, 300);
        float z = ofRandom(-500, 500);

        ofBoxPrimitive box;
        box.set(size);
        box.setPosition(x, y, z);
        boxes.push_back(box);

        // Gera uma cor aleatória e armazena no vetor
        ofColor color(ofRandom(255), ofRandom(255), ofRandom(255));
        colors.push_back(color);
    }
}

void ofApp::drawEquiScene() {
    ofPushStyle();
    ofSetLineWidth(3);

    // Desenha todos os cubos com as cores geradas na inicialização
    for (std::size_t i = 0; i < boxes.size(); i++) {
        ofSetColor(colors[i]); // Usa a cor correspondente do vetor
        boxes[i].draw();
    }

    ofPopStyle();
}

void ofApp::update() {
    em.render();
    em2.render();
    std::vector<string> faces = {"X+", "X-", "Y+", "Y-", "Z+", "Z-"};

    if (bSaveEquiRect) {
        ofPixels pixels;
        std::vector<ofFbo>& fbos = em2.getFbos(); // Pega os FBOs da `em2`
        for (std::size_t i = 0; i < fbos.size(); i++) {
            fbos[i].readToPixels(pixels); // Lê os pixels de cada FBO
            ofSaveImage(pixels, ofToString(i) + "_cubeMapFace_" + faces[i] + ".png"); // Salva a imagem
        }
        
        bSaveEquiRect = false; 
    }
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

void ofApp::keyPressed(int key) {
    if (key == 's') {
        bSaveEquiRect = true;
    }
}