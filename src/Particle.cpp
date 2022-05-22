#include "Particle.hpp"

/// Particle Constructor
Particle::Particle(vec2 initPos, float r)
: position(initPos), velocity(vec2(0, 0)), radius(r) {
      
      noiseSeed1 = ofRandomf();
      noiseSeed2 = ofRandomf();
    
}

/// Particle Destructor
Particle::~Particle() {
    
}

/// Make particle follow hand position
void Particle::update(vec2 handPos) {
    
    // Update noise value
    noise1 = ofNoise (noiseSeed1, ofGetElapsedTimef());
    noise2 = ofNoise (noiseSeed2, ofGetElapsedTimef());
    
    // Get initial velocity
    velocity = (handPos - getPosition()) * 0.1;
    
    // Add noise to velocity
    velocity += vec2(noise1, noise2) * 30 - vec2(15, 15);
    
    // Update position
    position += velocity;
    
}

/// Draw particle on screen
void Particle::draw() {
    
    ofPushStyle();
    ofSetColor(ofFloatColor(0.5, 0.0, 1.0));
    ofNoFill();
    ofDrawCircle(position, radius);
    ofPopStyle();
    
}

/// Get position of blob in openFrameworks
vec2 Particle::getPosition() {
    
    return position;
    
}

/// Get position of blob in screen (Y increases upwards)
vec2 Particle::getPixelPosition() {
    
    return vec2(position.x, ofGetHeight() - position.y);;
    
}

/// Get size of blob in pixelss
float Particle::getRadius() {
    
    return radius;
    
}
