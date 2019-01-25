#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"

#include "ofxFaceTracker.h"
#include "ofxBox2d.h"
#include "DizzyEye.hpp"

class Circle : public ofxBox2dCircle {
public:
    Circle(){};
    ofColor color;
    void drawParticle() {
        ofPushStyle();
        ofVec2f pos = getPosition();
        float r = getRadius();
        ofRectangle rect(pos.x - r/2, pos.y - r/2, r, r);
        ofFill();
        ofSetColor(color);
        draw();
        ofPopStyle();
    }
};

class Rectangle : public ofxBox2dRect {
public:
    Rectangle(){ };
    ofColor color;
    void drawParticle() {
        ofPushStyle();
        ofVec2f pos = getPosition();
        float w = getWidth();
        float h = getHeight();
        ofRectangle rect(pos.x, pos.y, w, h);
        ofFill();
        ofSetColor(color);
        draw();
        ofPopStyle();
    }
    
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
        void setObstacles();
    
        ofVideoGrabber cam;
        int camWidth;
        int camHeight;
        int effectNum;
        ofPixels viddata;
        ofTexture vidtexture;
        ofxCvColorImage vidimage;
        ofxCvGrayscaleImage grayvidimage;
        ofxFaceTracker tracker;
    
        // effect 0
        ofxBox2d box2d;
        vector <shared_ptr<Circle> > circles;
        vector <shared_ptr<Rectangle> > boxes;
        vector <shared_ptr<ofxBox2dEdge> > edges;
        ofxFaceTracker::Feature features [9] = {ofxFaceTracker::LEFT_EYE, ofxFaceTracker::RIGHT_EYE, ofxFaceTracker::LEFT_EYEBROW, ofxFaceTracker::RIGHT_EYEBROW, ofxFaceTracker::NOSE_BRIDGE, ofxFaceTracker::NOSE_BASE, ofxFaceTracker::INNER_MOUTH, ofxFaceTracker::OUTER_MOUTH, ofxFaceTracker::JAW };
        ofColor featureColors [9];
        ofRectangle featureBoxes[8];
        ofPoint faceCentroid;
    
        // effect 1
        ofPixels blurdata;
    
        // effect 2
        ofImage dizzyFrames[10];
        DizzyEye leftEye;
        DizzyEye rightEye;
        ofFbo faceMask;
        int faceRotateAng;
    
        // effect 3
        ofPolyline faceFeatures[4];
        int faceFeatureIndices[4];
};



