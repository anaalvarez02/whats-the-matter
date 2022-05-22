#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxAssimpModelLoader.h"
#include "Blob.hpp"
#include "Particle.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
        void exit();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
        // ––––– KINECT –––––
        
        ofxKinect kinect01;
        
        int angle;
        int minDist;
        int maxDist;
        
        vector<Blob*> blobs;
        
        
        // ––––– SHADER –––––
        
        ofShader metaballsShader;
        
        vec2 origin;
        
        vector<Particle*> metaballsParticles;
        vector<vec2> metaballsPositions;
        vector<float> metaballsSizes;
        
        
        // ––––– 3D MODELS –––––
        
        ofxAssimpModelLoader model;
        
        string selectedModel;
        
        bool chimeraDisplayed;
        bool chimeraActive;
        int chimeraCount;
        
        ofLight light;
        ofEasyCam cam;
    
};
