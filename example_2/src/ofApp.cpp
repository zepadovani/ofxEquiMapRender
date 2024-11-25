#include "ofApp.h"
/*
This example demonstrates how to use the ofxEquiMapRender class to render a scene in normal and equirectangular 
projection and save the cube map and equirectangular projection to image files.
*/


void ofApp::setup() {
    // Set vertical sync to true to avoid tearing
    ofSetVerticalSync(true);
    // Set the frame rate to 60 frames per second
    ofSetFrameRate(60);
    // Clear the screen with a black color
    ofClear(0);
    // Setup the equiMapRender with a resolution of 1024
    em.setup(1024, this);
    // Setup the custom FBO renderer with a resolution of 1024, GL_RGB format, and 4 samples
    em2.setup(1024, this, GL_RGB, 4);
    // Set the initial distance of the easyCam
    easyCam.setDistance(500);
    
    // Initialize flags for saving images and mode
    bSaveEquiRect = false;
    bSaveCubeMap = false;
    equiRectangularMode = false;

    // Create 30 random boxes with random sizes and positions
    for (int i = 0; i < 30; i++) {
        float size = ofRandom(50, 200);
        float x = ofRandom(-500, 500);
        float y = ofRandom(-300, 300);
        float z = ofRandom(-500, 500);

        ofBoxPrimitive box;
        box.set(size);
        box.setPosition(x, y, z);
        boxes.push_back(box);

        ofColor color(ofRandom(255), ofRandom(255), ofRandom(255));
        colors.push_back(color);
    }
}

void ofApp::drawEquiScene() {
    // Save the current style settings
    ofPushStyle();
    // Set the line width for drawing
    ofSetLineWidth(3);

    // Draw each box with its corresponding color
    for (std::size_t i = 0; i < boxes.size(); i++) {
        ofSetColor(colors[i]);
        boxes[i].draw();
    }
    // Restore the previous style settings
    ofPopStyle();
}

void ofApp::update() {
    // Render the equiMapRender if it is enabled
    if(em.isRenderEnabled()){
        em.render();
    }

    // Render the custom FBO renderer if it is enabled
    if(em2.isRenderEnabled()){
        em2.render();
    }

    // Define the names of the cube map faces
    std::vector<string> faces = {"X+", "X-", "Y+", "Y-", "Z+", "Z-"};

    // Save the cube map faces to image files if bSaveCubeMap is true
    if (bSaveCubeMap) {
        ofPixels pixels;
        std::vector<ofFbo>& fbos = em2.getSkyBoxFbos();
        for (std::size_t i = 0; i < fbos.size(); i++) {
            fbos[i].readToPixels(pixels);
            ofSaveImage(pixels, ofToString(i) + "_cubeMapFace_" + faces[i] + ".png");
        }
        
        bSaveCubeMap = false; 
    }

    // Save equirectangular projection to an image file if bSaveEquiRect is true
    if (bSaveEquiRect) {
        ofPixels pixels;
        em2.drawWarpedToFbo();
        ofFbo& em2fbo = em2.getWarpedFbo();
        em2fbo.readToPixels(pixels);
        ofSaveImage(pixels, "equiRectangular.png");
        bSaveEquiRect = false;   
    }
}

void ofApp::draw() {
    // Clear the screen with a black color
    ofClear(0);

    // Draw the scene in equirectangular mode or normal mode
    if(equiRectangularMode){
        // Draw the renderer or custom FBO renderer based on space key press
        if (ofGetKeyPressed(' ')) {
            em.draw(0, 0, ofGetWidth(), ofGetHeight());
            ofDrawBitmapStringHighlight("Renderer", 10, 40);
        } else {
            em2.draw(0, 0, ofGetWidth(), ofGetHeight());
            ofDrawBitmapStringHighlight("CustomFboRenderer", 10, 40);
        }
    } else {
        // Draw the scene using the easyCam
        easyCam.begin();
        drawEquiScene();
        easyCam.end();
    }

    // Display the current frame rate
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
}

void ofApp::keyPressed(int key) {
    // Set flags to save equirectangular and cube map images when 's' key is pressed
    if (key == 's') {
        bSaveEquiRect = true;
        bSaveCubeMap = true;
    };

    // Toggle equirectangular mode and adjust window shape when 'm' key is pressed
    if (key == 'm') {
        equiRectangularMode = !equiRectangularMode;
        if (equiRectangularMode) {
             ofSetWindowShape(2048, 1024); 
        } else {
            ofSetWindowShape(1088, 720);
        }
    }
}