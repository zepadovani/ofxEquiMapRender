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
    }

    void Renderer::render() {
        if (!scene) {
            return;
        }
        for (int i = 0; i < 6; i++) {
            cm.beginDrawingInto3D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i );
            ofClear(0);
            // work around for ofLight issue caused by ofxCubeMap
            ofLoadViewMatrix(ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
            scene->drawEquiScene();
            cm.endDrawingInto3D();
        }
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
    
    void CustomFboRenderer::setup(int size, Scene* s, int internalformat, int numSamples)
    {
        Renderer::setup(size, s, internalformat);
        // fbo.allocate(cm.getWidth(), cm.getHeight(), internalformat, numSamples);
        for(int i = 0; i < 6; i++) {
            ofFbo f;
            f.allocate(cm.getWidth(), cm.getHeight(), internalformat, numSamples);
            fbos.push_back(f);
        }
    }
    
    void CustomFboRenderer::setup(int size, Scene* s, ofFbo::Settings fbo_settings)
    {
        Renderer::setup(size, s, fbo_settings.internalformat);
        // fbo.allocate(fbo_settings);
        for(std::size_t i = 0; i < fbos.size(); i++) {
            fbos[i].allocate(fbo_settings);
        }
    }
    

    void CustomFboRenderer::render() {
        for (int i = 0; i < 6; ++i) {
            fbos[i].begin();
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
            fbos[i].end();
            
            cm.beginDrawingInto2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
            ofClear(0);
            fbos[i].draw(0, 0);
            cm.endDrawingInto2D();
        }
    }

    std::vector<ofFbo>& CustomFboRenderer::getFbos() {
        return fbos;
    }

    // const std::vector<ofFbo>& CustomFboRenderer::getFbos() {
    //     return fbos;
    // }
}