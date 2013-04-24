//
//  NSSharpy.cpp
//  NS2013
//
//  Created by Martial on 25/03/13.
//
//

#include "NSSharpy.h"

NSSharpy::NSSharpy() {
    
    maxRadius           = 4.f;
    brightness          = 1.0;
    
    rotationX           = 0.0;
    rotationY           = 0.0;
    
    decay               = 1.0;
    
    forcedBrightness    = -1.0;
    
      
}



void NSSharpy::setup(){
    
    target = new NSSharpyTarget();
    
    
    brtTween.setParameters(linear, ofxTween::easeOut, 0, 0, 1, 0);
    brtTween.start();
    
    reset();
    
}

void NSSharpy::reset(){
    
    this->goboPct   = 1.0;
    //this->pan       =
    
    
}

void NSSharpy::updateCylinder() {
    
    cylinder.clear();
    
    int rings = 32, resolution = 32;
    float length = 1024*4, radius = maxRadius * this->goboPct;
    
    
    for(int i = 0; i < rings; i++) {
        
        ofVec3f offset(0, 0, ofMap(i, 0, rings, 0, -length) );
        //offset.z -= length;
        for(int j = 0; j < resolution; j++) {
            float theta = ofMap(j, 0, resolution, 0, 360);
            ofVec2f cur(radius, 0);
            cur.rotate(theta);
            cylinder.addVertex(offset + cur);
            
        }
    }
    
    for(int i = 0; i < rings - 1; i++) {
        for(int j = 0; j < resolution; j++) {
            int sw = i * resolution + j, se = sw + 1;
            if(j + 1 == resolution) {
                se -= resolution;
            }
            int nw = sw + resolution, ne = se + resolution;
            cylinder.addTriangle(sw, se, nw);
            cylinder.addTriangle(nw, se, ne);
        }
    }
    
    cylinder.setMode(OF_PRIMITIVE_TRIANGLES);
    
    
}

//--------------------------------------------------------------


void NSSharpy::update(){
    
    // update mesh
    
    if(!cylinder.hasVertices() ) {
        updateCylinder();
    }
    
    // update props
    brtTween.setParameters(linear, ofxTween::easeOut, this->brightness, this->target->brt, decay, 0);
    this->goboPct       = 1.0;    
    this->brightness    = brtTween.update();
    
    transToTargetOrientation();
    
    sendToDmx();
    
    
    
    
}

void NSSharpy::transToTargetOrientation () {
        
    ofVec2f rot = this->target->setTweenedOrientation(rotationX, rotationY);    
    this->rotationX = rot.x;
    this->rotationY = rot.y;

    
}

void NSSharpy::sendToDmx() {
    
    // positions events
    
    vector<ofPoint> dmxData;
    
    // color
    dmxData.push_back(ofPoint(1, 8));
    
    // shutter    
    dmxData.push_back(ofPoint(2, 255));
    
    // dimmer
    dmxData.push_back(ofPoint(3, (int)(brightness * 255)));
    
    // Gobo
    dmxData.push_back(ofPoint(4, 0));
    
    // Prism
    dmxData.push_back(ofPoint(5, 0));
    
    // frost
    dmxData.push_back(ofPoint(8, 0));
    
    
    
    ofPoint panTilt = calculatePanTilt();
    
    dmxData.push_back(ofPoint(10, panTilt.x));
    
    dmxData.push_back(ofPoint(12, panTilt.y));
       
    ofNotifyEvent(dmxEvent, dmxData, this);

    
}

ofVec2f NSSharpy::calculatePanTilt() {
    

    
    /*
    ofMatrix4x4 m = this->getLocalTransformMatrix();
    ofVec3f t,s;
    ofQuaternion r,so;
    
    m.decompose(t, r, s, so);
    
    float pitch = atan2(2*(r.y()*r.z()+r.w()*r.x()),r.w()*r.w()-r.x()*r.x()-r.y()*r.y()+r.z()*r.z());
    float roll  = atan2(2*(r.x()*r.y()+r.w()*r.z()),r.w()*r.w()+r.x()*r.x()-r.y()*r.y()-r.z()*r.z());
    float yaw   = asin(-2*(r.x()*r.z()-r.w()*r.y()));
    
   
    
    if(id==-1) {
        printf("---------------------------------\n");
        printf("orx %f\n", orientation.x);
        printf("ory %f\n", orientation.y);
        printf("orz %f\n", orientation.z);
        printf("rx %f\n", rotationX);
        printf("rx %f\n", rotationY);
        
    }
    
    //float xRad = ofDegToRad(orientation.x);
    //float yRad = ofDegToRad(orientation.y);
    
    
    float xRad = ofDegToRad(rotationX);
    float yRad = ofDegToRad(rotationY);
     
    
    
     alpha = atan(tan(yRad) / tan(xRad));
     beta  = atan(sqrt(pow(tan(xRad),2) + pow(tan(yRad),2)));
    
    
    
    if(rotationX < 0 ) {
        //beta *= -1.0;
    }
    
    // convert into degress
    alpha   = ofRadToDeg(alpha);
    beta    = ofRadToDeg(beta);
    
      */
    alpha = rotationX;
    beta = rotationY;
    
    
     
    //alpha   = rotationY;
    //beta    = rotationX;
    
    /*
    float alpha = ofRadToDeg(pitch);
    float beta = ofRadToDeg(roll);
    */
    
    // --------------------- distance tests
    
    int distance = getAngleDIstance(currentAlpha, alpha);
    
    float nalpha =  alpha  + 180;
    float nbeta  =  beta * -1;
    

    int distance2 = getAngleDIstance(currentAlpha, nalpha);
    
    if(id==-1) {
        
        printf("---\n");
        printf("distance %d\n", distance);
        printf("distance2 %d\n", distance2);
        
    }
    
    if ( distance > distance2 ) {
        alpha = nalpha;
        beta = nbeta;        
    }
    
   

    
  
    
    // store old value
    currentAlpha = alpha;
    
    if(id==-1) {
        printf("---\n");
        printf("alpha %f\n", alpha);
        printf("beta %f\n", beta);
        
    }
    
    tmpParentNode.setPosition(this->getGlobalPosition());
       // map to shapry angles
    
    
    float pan   = ofMap(alpha, -90, 450, 0, 540);
    float tilt  = ofMap(beta, -90, 90, 45, 225);
    
    if(id==-1) {
    printf("---\n");
    printf("pan %f\n", pan);
    printf("tilt %f\n", tilt);
        
    }
    
    

    
    // map to DMX values
    
    pan     = ofMap(pan, 0, 540, 0, 255);
    tilt    = ofMap(tilt, 0, 270, 0, 255);
    
    return ofVec2f(pan, tilt);
    
}


int NSSharpy:: getAngleDIstance(float a, float b) {
    
    double diff = (a > b ? a - b : b - a);
    double mod_diff = fmod(diff, 360);
    double result = (mod_diff < 180 ? mod_diff : 360 - mod_diff);
    
};


//--------------------------------------------------------------


void NSSharpy::draw(){
    
   // ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ofSetColor(200);
    
    transformGL();
    ofPushMatrix();
    
    ofRotateZ(alpha);
    
    //printf("brt %f", brightness);
    
    ofPushMatrix();
    ofRotateX(beta);
    ofSetColor(255, 255, 255, brightness * 255.f);
    if(brightness > 0.0)
    cylinder.draw();
    ofPopMatrix();
    ofPopMatrix();
    
    
    ofSetColor(255, 255, 255, 255.f);
    ofPushMatrix();
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    ofDrawBitmapString(ofToString(id), 0,0);
    ofPopMatrix();
    
    
    restoreTransformGL();
    
}

//--------------------------------------------------------------

ofVec3f NSSharpy::getEulerDistance() {
    
    ofVec3f currentEuler    = this->getOrientationEuler();
    ofVec3f targetEuler     = this->target->getOrientationEuler();
    
    if(id == 0) {
        
        // pan, tilt, roll
        
        ofVec3f distance = targetEuler - currentEuler;
        distance.x = abs(distance.x);
        distance.y = abs(distance.y);
        distance.z = abs(distance.z);
        
       // ofLog(OF_LOG_NOTICE, "Distance x : %f, y : %f, z : %f", distance.x, distance.y, distance.z);
                
    }
    
    
}

//--------------------------------------------------------------

void NSSharpy::setID(int id){
    
    this->id = id;
    
}

void NSSharpy::setTargetOrientation(ofVec3f orientation) {
    this->target->setOrientation(orientation);
}


void NSSharpy::setBrightness(float brighntessPct){
    //this->brightness = brighntessPct;
    //this->target->brt = brighntessPct;
    this->forcedBrightness = brighntessPct;
    
}

float NSSharpy::getBrightness(){
    return this->brightness;    
}

void NSSharpy::setGobo(float pct){
     this->target->goboPct = pct;
    //this->goboPct = pct;
}

float NSSharpy::getGobo(){
    return this->goboPct;
}