#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // ––––– KINECT –––––
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // Initialise Kinect
    kinect01.setRegistration(true);
    
    kinect01.init();
    kinect01.open();
    
    if(kinect01.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect01.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect01.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect01.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect01.getZeroPlaneDistance() << "mm";
    }
    
    // Set angle and depth threshold of Kinect
    angle = 10;
    minDist = 800;
    maxDist = 1000;
    
    
    // ––––– SHADER –––––
    
    ofDisableArbTex();
    ofEnableNormalizedTexCoords();

    // Load shader
    metaballsShader.load("metaballs");
    
    // Set origin coordinates of particle system
    origin = vec2(ofGetWidth() / 2, ofGetHeight() / 2);
    
    // Create 20 particles in center with random size
    for (int i = 0; i < 20; i++) {
        Particle* curP = new Particle{origin, ofRandom(5, 15)};
        metaballsParticles.push_back(curP);
        
        // Add particle sizes & positions to respective vectors
        metaballsSizes.push_back(curP->getRadius());
        metaballsPositions.push_back(curP->getPixelPosition());
    }
    
    
    // ––––– 3D MODELS –––––
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetGlobalAmbientColor(ofFloatColor(0.4));
    ofSetSmoothLighting(true);
    ofEnableNormalizedTexCoords();
    
    // Set light
    light.setPosition(200, 200, 200);
    light.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
    light.setSpecularColor(light.getDiffuseColor());
    
    // Set camera
    cam.setAutoDistance(false);
    cam.setPosition(0, 0, 200);
    cam.disableMouseInput();
    
    // Set initial model
    selectedModel = "chimera3";
    model.loadModel(selectedModel+"_2.dae", false);
    model.setScale(0.075, 0.075, 0.075); // Size modified to fit in exhibition screen
    model.setRotation(1, 180, 0, 0, 1);
    model.setPosition(0, -20, 0);
    
    // Set initial model state
    chimeraDisplayed = true;
    chimeraCount = 0;
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    
    // Delete blobs and particles
    for (auto& b : blobs) delete b;
    for (auto& m : metaballsParticles) delete m;
    
    // Reset and shut down kinect
    kinect01.setCameraTiltAngle(0);
    kinect01.close();
    
}

//--------------------------------------------------------------
void ofApp::update(){

    // ––––– KINECT & SHADER –––––
    
    // Reset blobs and saved particle positions
    blobs.clear();
    metaballsPositions.clear();
    
    // Reset blob detection bool
    bool blobDetected = false;
    
    // Update kinect
    kinect01.setCameraTiltAngle(angle);
    kinect01.update();
    
    // Detect blobs using depth data
    for (float x = 0; x < kinect01.width; x++) {
        for (float y = 0; y < kinect01.height; y++) {
            int index = x + y * kinect01.width;

            float d = kinect01.getDistanceAt(x, y);
            
            float xPix = ofGetWidth() - x * ofGetWidth() / kinect01.width;
            float yPix = y * ofGetHeight() / kinect01.height;
            
            // If within the depth threshold
            if (d > minDist && d < maxDist) {
                
                bool blobFound = false;
                
                // Add pixel to blob if near enough
                for (auto& b : blobs) {
                    if (b->isNear(xPix, yPix)) {
                        b->add(xPix, yPix);
                        blobFound = true;
                        break;
                    }
                }
                
                // If not, create new blob
                if (!blobFound) {
                    blobs.push_back(new Blob{xPix, yPix});
                }
            }
        }
    }
    
    for (float i = 0; i < 20; i++) {
        // Each particle follows a blob
        if (i < blobs.size()) {
            metaballsParticles[i]->update(blobs[i]->getPosition());
        }

        // If there are more particles than blobs, the extra particles remain in the center
        else metaballsParticles[i]->update(origin);
        
        // Update
        metaballsPositions.push_back(metaballsParticles[i]->getPixelPosition());
    }
    
    
    // ––––– 3D MODELS –––––
    
    // Check for blob bigger than 100px detected
    for(auto& b : blobs) {
        if (b->getSize() > 100) blobDetected = true;
    }
    
    // If so animate chimera
    if (blobDetected) chimeraActive = true;
    
    // If animation count reaches 121 & no blobs detected
    if (!blobDetected && chimeraActive && chimeraCount > 121) {
        
        // Load to (new) random chimera model
        selectedModel = "chimera" + std::to_string(int(ofRandom(1, 4)));
        model.loadModel(selectedModel+"_2.dae", false);
        
        // Set scale according to model
        if(selectedModel == "chimera3") model.setScale(0.075, 0.075, 0.075);
        else if(selectedModel == "chimera2") model.setScale(0.045, 0.045, 0.045);
        else model.setScale(0.05, 0.05, 0.05);
        
        // Set other model properties
        model.setRotation(1, 180, 0, 0, 1);
        model.setLoopStateForAllAnimations(OF_LOOP_NONE);
        model.resetAllAnimations();
        model.playAllAnimations();
        
        // Display model again
        chimeraDisplayed = true;
        
        // Stop & reset animation count
        chimeraActive = false;
        chimeraCount = 0;
    }

    // If animating, update count
    if (chimeraActive) chimeraCount++;
    
    switch (chimeraCount) {
            
        case 1: // When animation starts, load model explosion
            model.loadModel(selectedModel+"_1.dae", false);
            model.setScale(1, 1, 1);
            model.setRotation(1, 180, 0, 0, 1);
            
            model.setLoopStateForAllAnimations(OF_LOOP_NONE);
            model.resetAllAnimations();
            model.playAllAnimations();
            
            break;

        case 101: // When animation count reaches 101, hide model
            model.stopAllAnimations();
            chimeraDisplayed = false;
            break;
            
        default:
            break;
    }
    
    // Update model animation
    model.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    // ––––– SHADER –––––
    
    metaballsShader.begin();

    // Pass sizes & positions of particles onto shader
    metaballsShader.setUniform1fv("mSizes", &metaballsSizes[0], metaballsSizes.size());
    metaballsShader.setUniform2fv("mPositions", &metaballsPositions[0].x, metaballsPositions.size());

    // Pass colours of mesh onto shader
    metaballsShader.setUniform4f("innerColor", vec4(.9, .93, .84, 1.));
    metaballsShader.setUniform4f("outerColor", vec4(.73, .77, .67, 1.));

    // Apply shader to rectangle to display on screen
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    metaballsShader.end();
    
    
    // ––––– SHADER LINKS –––––
    
    // For every particles, draw 2 white lines connecting it with 2 other random ones
    ofPushStyle();
    ofSetColor(255);
    ofSetLineWidth(3);
    
    for (auto& m : metaballsParticles) {
        vec2 curPos = m->getPosition();
        
        int r1 = int(ofRandom(0, 20));
        ofDrawLine(curPos, metaballsParticles[r1]->getPosition());
        
        int r2 = int(ofRandom(0, 20));
        ofDrawLine(curPos, metaballsParticles[r2]->getPosition());
    }
    
    ofPopStyle();
    
    
    // ––––– 3D MODELS –––––
    
    // Draw chimera model when display bool is true
    if(!chimeraDisplayed) return;
    
    ofEnableDepthTest();
    cam.begin();
    ofEnableLighting();
    light.enable();
    
    model.drawFaces();
    
    light.disable();
    ofDisableLighting();
    cam.end();
    ofDisableDepthTest();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    // Update angle of Kinect
    switch (key) {
            
        case OF_KEY_UP:
            if (angle < 30) angle++;
            kinect01.setCameraTiltAngle(angle);
            break;
            
        case OF_KEY_DOWN:
            if (angle > -30) angle--;
            kinect01.setCameraTiltAngle(angle);
            break;
            
        default:
            break;
            
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

    origin = vec2(ofGetWidth() / 2, ofGetHeight() / 2);
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
