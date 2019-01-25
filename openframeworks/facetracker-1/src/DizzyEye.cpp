//
//  DizzyEye.cpp
//  facetracker-1
//
//  Created by Lo Wing Ellen on 1/22/19.
//

#include "DizzyEye.hpp"

void DizzyEye::setup(ofImage * frames_, int framesSize_) {
    gifindex = 0;
    gifsize = framesSize_;
    for(int i = 0; i < gifsize; i++) {
        frames.push_back(frames_[i]);
    }
    scale = 2.0;
}

void DizzyEye::update(int x_, int y_, int w_, int h_) {
    if(ofGetElapsedTimeMillis() % 3) {
        gifindex++;
        if(gifindex > gifsize-1) gifindex = 0;
    }
    x = x_;
    y = y_;
    w = w_;
    h = h_;
    
    frame = frames[gifindex];
    rectSize = (w > h) ? w * scale : h * scale;
    frame.resize(rectSize, rectSize);
}

ofPixels & DizzyEye::draw() {
    ofPixels & framepixels = frame.getPixels();
    return framepixels;
}
