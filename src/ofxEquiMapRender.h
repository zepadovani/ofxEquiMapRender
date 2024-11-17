#pragma once

#include "ofMain.h"
#include "ofxCubeMapOld.h"

namespace ofxEquiMapRender
{
    class Scene
    {
    public:
        virtual ~Scene() {}
        virtual void drawEquiScene() = 0;
    };

       
    class Renderer
    {
    protected:
        Scene* scene = NULL;
        ofxCubeMapOld cm;
        ofShader warpShader;
    public:
        void setup(int size, Scene* s, int internalformat = GL_RGB);
        virtual void render();
        void draw(float x, float y, float w, float h);
            
        void setPosition(const ofVec3f& p);
        void registerScene(Scene* s);
        void setPosition(float x, float y, float z);
        ofxCubeMapOld& getCubeMap();

        

        // ofPixels getPixels() {
        //     ofPixels pixels;
        //     cm.getFacePixels(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, pixels);
        //     return pixels;
        // }
        
    };
    
    class CustomFboRenderer : public Renderer
    {
    protected:
        // ofFbo fbo;
        std::vector<ofFbo> fbos;
    public:
        void setup(int size, Scene* s, int internalformat = GL_RGB, int numSamples = 0);
        void setup(int size, Scene* s, ofFbo::Settings fbo_settings);
        void render() override;
        
        // ofFbo& getFbo() { return fbo; }
        // const ofFbo& getFbo() const { return fbo; }

        std::vector<ofFbo>& getFbos();
        // const std::vector<ofFbo>& getFbos();
    };
};