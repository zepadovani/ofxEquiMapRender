#include "ofxEquiMapRender.h"

#ifndef STRINGIFY
#define STRINGIFY(A) #A
#endif

namespace ofxEquiMapRender {
    static string warp_frag_shader_str = STRINGIFY(
       uniform samplerCube envMap;
       
       void main() {
           
           vec2 tc = gl_TexCoord[0].st / vec2(2.0) + 0.5;  //only line modified from the shader toy example
           vec2 thetaphi = ((tc * 2.0) - vec2(1.0)) * vec2(3.1415926535897932384626433832795, 1.5707963267948966192313216916398);
           vec3 rayDirection = vec3(cos(thetaphi.y) * cos(thetaphi.x), sin(thetaphi.y), cos(thetaphi.y) * sin(thetaphi.x));
           
           gl_FragColor = textureCube(envMap, rayDirection);
       }
       );
    
    void Renderer::setup(int size, Scene* s, int internalformat)
    {
        int type = ofGetGlTypeFromInternal(internalformat);
        int format = ofGetGLFormatFromInternal(internalformat);
        cm.initEmptyTextures(size, format, type);
        warpShader.setupShaderFromSource(GL_FRAGMENT_SHADER, warp_frag_shader_str);
        warpShader.linkProgram();
        registerScene(s);

        warpedFbo.allocate(size*2, size, internalformat);
        
    }

    void Renderer::setRenderEnabled(bool enable) {
        renderEnabled = enable;
    }

    bool Renderer::isRenderEnabled() const {
        return renderEnabled;
    }


    void Renderer::renderToCubeMap() {
        if (!scene || !renderEnabled) {
            return;
        }
        
        for (int i = 0; i < 6; i++) {
            cm.beginDrawingInto3D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
            ofClear(0);
            ofLoadViewMatrix(ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
            scene->drawEquiScene();
            cm.endDrawingInto3D();
        }
    }

    void Renderer::drawWarpedToFbo() {
            if (!scene || !renderEnabled) {
                return;
            }
            warpedFbo.begin();
            ofClear(0); // Clear the FBO
            warpShader.begin();
            warpShader.setUniform1i("envMap", 0);
            cm.drawFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, warpedFbo.getWidth(), warpedFbo.getHeight()); // Draw to FBO's dimensions
            warpShader.end();
            warpedFbo.end();
    }

    void Renderer::drawWarpedFbo(float x, float y, float w, float h) {
            // Draw the FBO's contents to the screen
            warpedFbo.draw(x, y, w, h); 
        }

    void Renderer::render() {
        renderToCubeMap();
    }
    
    void Renderer::draw(float x, float y, float w, float h) {
        warpShader.begin();
        warpShader.setUniform1i("envMap", 0);
        cm.drawFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, x, y, w, h);
        warpShader.end();




    }

    void Renderer::setPosition(const ofVec3f& p) {
    ofVec3f p2 = p;
    cm.setPosition(p2);
    }

    void Renderer::registerScene(Scene* s) {
        scene = s;
    }

    void Renderer::setPosition(float x, float y, float z) {
        cm.setPosition(x, y, z);
    }

    ofxCubeMapOld& Renderer::getCubeMap() {
        return cm;
    }

    std::vector<ofFbo>& Renderer::getSkyBoxFbos() {
        return skyBoxFbos;
    }

    ofFbo& Renderer::getWarpedFbo() {
        return warpedFbo;
    }
    

    
    void CustomFboRenderer::setup(int size, Scene* s, int internalformat, int numSamples)
    {
        Renderer::setup(size, s, internalformat);
        // fbo.allocate(cm.getWidth(), cm.getHeight(), internalformat, numSamples);
        for(int i = 0; i < 6; i++) {
            ofFbo f;
            f.allocate(cm.getWidth(), cm.getHeight(), internalformat, numSamples);
            skyBoxFbos.push_back(f);
        }
    }
    
    void CustomFboRenderer::setup(int size, Scene* s, ofFbo::Settings fbo_settings)
    {
        Renderer::setup(size, s, fbo_settings.internalformat);
        // fbo.allocate(fbo_settings);
        for(std::size_t i = 0; i < skyBoxFbos.size(); i++) {
            skyBoxFbos[i].allocate(fbo_settings);
        }
    }

    void CustomFboRenderer::render() {
        for (int i = 0; i < 6; ++i) {
            skyBoxFbos[i].begin();
            ofClear(0);
            ofPushView();
            
            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            glLoadMatrixf( cm.getProjectionMatrix().getPtr() );
            
            glMatrixMode( GL_MODELVIEW );
            glLoadIdentity();
            glLoadMatrixf( cm.getLookAtMatrixForFace( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i).getPtr() );
            
            // work around for ofLight issue caused by ofxCubeMap
            ofLoadViewMatrix(ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
            
            scene->drawEquiScene();
            
            ofPopView();
            skyBoxFbos[i].end();
            
            cm.beginDrawingInto2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
            ofClear(0);
            skyBoxFbos[i].draw(0, 0);
            cm.endDrawingInto2D();
        }
    }
}