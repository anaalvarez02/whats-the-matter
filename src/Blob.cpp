#include "Blob.hpp"

/// Blob Constructor
Blob::Blob(float x, float y)
: minPix(x, y), maxPix(x, y), threshold(150) {
    
    // Calculate center coordinates
    float centerX = minPix.x + (maxPix.x - minPix.x) / 2;
    float centerY = minPix.y + (maxPix.y - minPix.y) / 2;
    centerPix = vec2(centerX, centerY);
    
}

/// Blob Destructor
Blob::~Blob() {
    
}

/// Detect if pixel is near other center of blob
bool Blob::isNear(float otherX, float otherY) {
    
    float d = distance(centerPix, vec2(otherX, otherY));
    
    if (d < threshold) return true;
    else return false;
    
}

/// Add new pixel to blob
void Blob::add(float newX, float newY) {
    
    // Add new pixel to blob by updating max and min pixels
    vec2 newPix = vec2(newX, newY);
    minPix = min(minPix, newPix);
    maxPix = max(maxPix, newPix);
    
    // Update center coordinates
    float centerX = minPix.x + (maxPix.x - minPix.x) / 2;
    float centerY = minPix.y + (maxPix.y - minPix.y) / 2;
    centerPix = vec2(centerX, centerY);
    
}

/// Draw blob on screen
void Blob::draw() {
    
    ofPushStyle();
    ofFill();
    ofDrawCircle(centerPix, getSize() / 2);
    ofPopStyle();
    
}

/// Get size of blob in pixels
float Blob::getSize() {
    
    return distance(minPix, maxPix);
    
}

/// Get coordinates of blob center
vec2 Blob::getPosition() {
    
    return centerPix;
    
}
