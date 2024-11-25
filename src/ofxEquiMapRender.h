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
        Scene* scene = nullptr;
        ofxCubeMapOld cm;
        ofShader warpShader;
        bool renderEnabled = true; 
        std::vector<ofFbo> skyBoxFbos;
        ofFbo warpedFbo;
    public:
        void setup(int size, Scene* s, int internalformat = GL_RGB);
        virtual void render();
        void draw(float x, float y, float w, float h);
            
        void setPosition(const ofVec3f& p);
        void registerScene(Scene* s);
        void setPosition(float x, float y, float z);
        ofxCubeMapOld& getCubeMap();

        void setRenderEnabled(bool enable);
        bool isRenderEnabled() const;
        void renderToCubeMap();

        void drawWarpedToFbo();
        void drawWarpedFbo(float x, float y, float w, float h);
        
        
        std::vector<ofFbo>& getSkyBoxFbos();
        ofFbo& getWarpedFbo();

        
        
    };
    
    class CustomFboRenderer : public Renderer
    {
    // protected:
    //     // ofFbo fbo;
    //     // std::vector<ofFbo> skyBoxFbos;
    public:
        void setup(int size, Scene* s, int internalformat = GL_RGB, int numSamples = 0);
        void setup(int size, Scene* s, ofFbo::Settings fbo_settings);
        void render() override;
           
    };
};