#pragma once

#include "ofMain.h"

using namespace glm;

class Blob {

    public:
        Blob(float x, float y);
        ~Blob();
        
        // Functions to update blob
        bool isNear(float otherX, float otherY);
        void add(float newX, float newY);
        void draw();
        
        // Functions to get blob info
        float getSize();
        vec2 getPosition();
        
    private:
        // Blob position
        vec2 minPix;
        vec2 maxPix;
        vec2 centerPix;
        
        // Maximum distance between blob points
        float threshold;
    
};
