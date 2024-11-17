#include "ofApp.h"

void ofApp::setup() {
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofClear(0);
    em.setup(1024, this);
    em2.setup(1024, this, GL_RGB, 4);
    easyCam.setDistance(500); // Inicializa a easyCam
    
    bSaveEquiRect = false;
    bSaveCubeMap = false;
    equiRectangularMode = false;

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
    if(em.isRenderEnabled()){ // Só renderiza o cubemap se estiver habilitado
        em.render(); // Renderiza para o cubemap (chama renderToCubeMap() internamente)
    }

    if(em2.isRenderEnabled()){ // Só renderiza o cubemap se estiver habilitado
        em2.render(); // Renderiza para o cubemap (chama renderToCubeMap() internamente)
    }
    // em.render();
    // em2.render();
    std::vector<string> faces = {"X+", "X-", "Y+", "Y-", "Z+", "Z-"};


    if (bSaveCubeMap) {
        ofPixels pixels;
        std::vector<ofFbo>& fbos = em2.getFbos(); // Pega os FBOs da `em2`
        for (std::size_t i = 0; i < fbos.size(); i++) {
            fbos[i].readToPixels(pixels); // Lê os pixels de cada FBO
            ofSaveImage(pixels, ofToString(i) + "_cubeMapFace_" + faces[i] + ".png"); // Salva a imagem
        }
        
        bSaveCubeMap = false; 
    }

    if (bSaveEquiRect) {
        ofPixels pixels;
        em2.drawWarpedToFbo();
        ofFbo& em2fbo = em2.getWarpedFbo();
        em2fbo.readToPixels(pixels);
        ofSaveImage(pixels, "equiRectangular.png");
        // em2.getCubeMap().getFaceFbo(GL_TEXTURE_CUBE_MAP_POSITIVE_Z).readToPixels(pixels); // Usa em2!
        // ofSaveImage(pixels, "equiRectangular.png");
        // bSaveEquiRect = false;
    
    }

    

    // if (bSaveCubeMap) {
    //     ofPixels pixels;
    //     std::vector<string> faces = {"X+", "X-", "Y+", "Y-", "Z+", "Z-"};
    //     for (int i = 0; i < 6; i++) {
    //         // Corrigido: Usa getFaceFbo() do CustomFboRenderer (em2)
    //         em2.getCubeMap().getFaceFbo(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i).readToPixels(pixels); 
    //         ofSaveImage(pixels, ofToString(i) + "_cubeMapFace_" + faces[i] + ".png"); 
    //     }
    //     bSaveCubeMap = false;
    // }
}

void ofApp::draw() {
    ofClear(0);

    if(equiRectangularMode){

        if (ofGetKeyPressed(' ')) {
            em.draw(0, 0, ofGetWidth(), ofGetHeight());
            ofDrawBitmapStringHighlight("Renderer", 10, 40);
        } else {
            em2.draw(0, 0, ofGetWidth(), ofGetHeight());
            ofDrawBitmapStringHighlight("CustomFboRenderer", 10, 40);
        }
    } else {
        easyCam.begin();
        drawEquiScene();
        easyCam.end();
    }

    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
}

void ofApp::keyPressed(int key) {
 
    if (key == 's') {
        bSaveEquiRect = true;
        bSaveCubeMap = true;
    };
    
    if (key == 'm') { // Alterna entre os modos de visualização com a tecla 'm'
        equiRectangularMode = !equiRectangularMode;
        if (equiRectangularMode) {
            // Maximiza a janela para o modo equirretangular
             ofSetWindowShape(2048, 1024); // Resolução típica equirretangular
        } else {
           // Volta para as dimensões padrão (defina as dimensões desejadas)
            ofSetWindowShape(1088, 720);
        }
    }
}