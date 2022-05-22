#pragma once

#include "ofMain.h"

using namespace glm;

class Particle {
    
    public:
        Particle(vec2 initPos, float r);
        ~Particle();
        
        // Functions to update particle
        void update(vec2 handPos);
        void draw();
        
        // Functions to get particle info
        vec2 getPosition();
        vec2 getPixelPosition();
        float getRadius();
        
    private:
        // Variables for particle movement
        vec2 position;
        vec2 velocity;
        float noiseSeed1, noiseSeed2, noise1, noise2;
        
        // Variable for particle size
        float radius;
    
};
